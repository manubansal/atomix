#!/usr/bin/python
'''
Atomix project, _scrape_b_i.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''


import csv
import os
import sys
import re
from _util import *


class BlockScraper:

  def which(self, blockname, ilib_include_path):
    inc_list = []
    if blockname == "BlockNOP":
      return ""
    for path in ilib_include_path:
      fname = '%s/%s_i.h' % (path, blockname)
      if os.path.exists(fname):
	self.logger.debug('%s exists' % fname)
	inc_list.append(fname)
      else:
	pass

    if len(set(inc_list)) > 1:
      print_line()
      self.logger.error("Multiple block include files found for %s" % blockname)
      print_line()
      exit(17)
    elif len(inc_list) == 0:
      print_line()
      self.logger.error("No block include file found for %s" % blockname)
      print_line()
      exit(18)

    return inc_list[0]

  def parse_block_i_h_file(self, blockname, block_i_h_file):
    if blockname == "BlockNOP":
      return ([], [], [], [], [], [], [])
    ifile = open(block_i_h_file,'r')
    text = ifile.read()
    ifile.close()

    (inp_list, out_list, cf_list, inp_list2, out_list2) = self.find_declaration_i(blockname, text)
    if cf_list:
      cf_par_list, cf_par_dict = self.find_declaration_i_conf(blockname, text)
    else:
      cf_par_list = []
      cf_par_dict = {}
    return (inp_list, out_list, cf_list, inp_list2, out_list2, cf_par_list, cf_par_dict)

  def find_declaration_i(self, blockname, text):
    pattern = re.compile(r'%s_i\s*\(.*?\;' % blockname, re.VERBOSE | re.DOTALL) 

    count = 0
    for match in pattern.finditer(text):
	count += 1
	if count > 1:
	  print_line()
	  self.logger.error('More than one implementation prototype declarations found for %s' % blockname)
	  print_line()
	  exit(1)
	text = match.group(0)
    if count == 0:
      print_line()
      self.logger.error('No implementation prototype declaration found for %s' % blockname)
      print_line()
      exit(1)

    subpattern = re.compile(r'(IN|OUT|CF)\s+([a-zA-Z_\d]+)\s*(?:const)?\s*\*\s*(?:const)?\s*(?:restrict)?\s*([a-zA-Z_\d]+)', re.VERBOSE) 

    inp_list = []
    out_list = []
    cf_list = []
    inp_list2 = []
    out_list2 = []

    for line in text.split('\n'):
      for match in subpattern.finditer(line):
	iodir  = match.group(1)
	iotype = match.group(2)
	ioname = match.group(3)
	if (iodir == 'IN'):
	  inp_list.append(iotype)
	  inp_list2.append((iotype, ioname))
	elif (iodir == 'OUT'):
	  out_list.append(iotype)
	  out_list2.append((iotype, ioname))
	elif (iodir == 'CF'):
	  cf_list.append(iotype)
	else:
	  print_line()
	  self.logger.error('Header file parse error')
	  print_line()
	  exit(1)

    return (inp_list, out_list, cf_list, inp_list2, out_list2)

  def find_declaration_i_conf(self, blockname, text):
    pattern = re.compile(r'%s_i_conf\s*\(.*?\;' % blockname, re.VERBOSE | re.DOTALL) 

    count = 0
    for match in pattern.finditer(text):
	count += 1
	if count > 1:
	  self.logger.error('More than one configuration implementation prototype declarations found for %s' % blockname)
	  exit(1)
	text = match.group(0)
    if count == 0:
      print_line()
      self.logger.error('No configuration implementation prototype declaration found for %s' % blockname)
      print_line()
      exit(1)

    subpattern_cf = re.compile(r'(CF)\s+([a-zA-Z_\d]+)\s*(?:const)?\s*\*\s*(?:const)?\s*(?:restrict)?\s*([a-zA-Z_\d]+)', re.VERBOSE) 
    subpattern_par = re.compile(r'([a-zA-Z_\d]+)\s*(?:const)?\s*(?:const)?\s*([a-zA-Z_\d]+)', re.VERBOSE) 

    par_list = []
    par_dict = {}

    lines = text.split('\n')
    line = lines[1]

    count = 0
    for match in subpattern_cf.finditer(line):
      count += 1
    if count == 0:
      print_line()
      self.logger.error('Configuration implementation prototype must have CF as the first parameter')
      print_line()
      exit(2)


    for line in lines[2:]:
      for match in subpattern_par.finditer(line):
	iotype = match.group(1)
	ioname = match.group(2)
	par_dict[ioname] = iotype
	par_list.append(ioname)

    return (par_list, par_dict)

  def __init__(self, logger):
    self.logger = logger
