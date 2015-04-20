#!/usr/bin/python
'''
Atomix project, orpy.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

import sys
import os
from optparse import (OptionParser,BadOptionError,AmbiguousOptionError)
import importlib

from orpy_util import *
#from genatomstxt import *
from genstatestxt import *

LOGFILE="orpy.log"

class MyOptionParser(OptionParser):
  def error(self, msg):
    pass

class PassThroughOptionParser(OptionParser):
    """
    An unknown option pass-through implementation of OptionParser.

    When unknown arguments are encountered, bundle with largs and try again,
    until rargs is depleted.  

    sys.exit(status) will still be called if a known argument is passed
    incorrectly (e.g. missing arguments or bad argument types, etc.)        
    """
    def _process_args(self, largs, rargs, values):
        while rargs:
            try:
                OptionParser._process_args(self,largs,rargs,values)
            except (BadOptionError,AmbiguousOptionError), e:
                largs.append(e.opt_str)
def create_hwmgr(hw_info_file_path):
  hwmgr = OrHWMgr(hw_info_file_path)
  return hwmgr  
    
def import_app(app_dir, app_name):
  print 'Importing app %s from %s' % (app_name, app_dir)
  sys.path.append(app_dir)
  app_module = importlib.import_module(app_name)
  print 'Imported app module %s from %s' % (app_name, app_dir)
  print app_module
  app_class = getattr(app_module, app_name)
  print app_class
  return app_class

def create_app(app_class):
  print 'Creating app instance'
  app = app_class()
  print 'Created app instance'
  print 'Invoking app declaration'
  app.declareApp()
  print 'Populated app from declaration'
  return app

def validate_app(app):
  pass

def init_file():
  #This is where we will init any intermediate files
  #that need to be written. Right now, none
  pass

def genlp(hwMgr, app):
  init_file()
  q = hwMgr.getQ()
  states = app.getStates();
  for state in states:
    action = state.getAction()
    g = action.getGraph()
    print("Graph is ..\n")
    for row in g:
      print("%d %d\n"%(row[0], row[1]))  
    
    p = action.getBenchMark(hwMgr)
    for row in p:
      print("%d %d\n"%(row[0], row[1]))
    #q = action.getQMatrix()
    write_cplex_input(state, g, p, q)
    run_cplex(state)
    parse_cplexsol(state)

def schedule_app(hwMgr, app):
  Graph_Sched = genGraphforSched(app)
  genlp(hwMgr, app)    

def benchmark_app(hwMgr, app):
  write_benchmark_file(hwMgr, app);         

def main(logger):

  parser = PassThroughOptionParser()
  parser.add_option("-I", "--include", dest="ilib_include_path",
            help="include path", metavar="ILIB_INCLUDE_PATH",action='append')

  (options, args) = parser.parse_args(sys.argv)
  ilib_include_path = options.ilib_include_path

  app_name = args[1]
  app_dir = args[2]
  print 'app_dir = ', app_dir
  using_blocks = args[3:]

  hw_desc_file_path = app_dir+'/hw_info';
  import_blocks(logger, using_blocks, ilib_include_path)
  app_class = import_app(app_dir, app_name)
  app = create_app(app_class)
  hwMgr = create_hwmgr(hw_desc_file_path);  
  benchmark_app(hwMgr, app)    
  validate_app(app)
  schedule_app(hwMgr, app)
  createAtoms(hwMgr, app) # core assignments are stored in app

  # Test this atoms list
  for at in app.atoms:
    print("ATOM : %s %d %d %s\n" %(at.block, at.coreID, at.stateID, at.atomName));

  makeR2Graph(app, hwMgr)
  genStatesTxt(app, hwMgr)  
     
   
if __name__ == "__main__":
  logger = create_logger(LOGFILE)
  logger.info('invoking orpy compiler')
  main(logger)
