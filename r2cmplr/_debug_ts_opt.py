'''
Atomix project, _debug_ts_opt.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from _util import *
from _timestamp_config import *
import re

class DebugTsOpt:

  def get_debug_conf(self, debug_conf_file, timestamp_conf):
    #parse file
    f = open(debug_conf_file, 'r')
    lines = [l.strip() for l in f.readlines()]
    f.close()

    #atoms to debug
    lines = [l for l in lines if (not (l.startswith('#') or l == ''))]
    atoms_to_debug = [l.split(':')[1].strip() for l in lines if l.startswith('debug')]
    self.logger.info("atoms_to_debug: %s" % atoms_to_debug)

    #atoms to identify
    lines = [l for l in lines if (not (l.startswith('#') or l == ''))]
    atoms_to_identify = [l.split(':')[1].strip() for l in lines if l.startswith('identify')]
    self.logger.info("atoms_to_identify: %s" % atoms_to_identify)

    #time stamp conf
    self.TIMESTAMP_CONFIG = populate_timestamp_config(timestamp_conf)

    if self.TIMESTAMP_CONFIG['TIMESTAMP_STATES']:
      states_to_timestamp = [l.split(':')[1].strip() for l in lines if l.startswith("ts_st")]
    else:
      states_to_timestamp = []
    self.TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = states_to_timestamp
    self.logger.info('states_to_timestamp: %s' % states_to_timestamp)

    if self.TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS']:
      actions_to_timestamp = [l.split(':')[1].strip() for l in lines if l.startswith("ts_ax")]
    else:
      actions_to_timestamp = []
    self.TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS_TO_TIMESTAMP'] = actions_to_timestamp
    self.logger.info('actions_to_timestamp: %s' % actions_to_timestamp)

    #axn_scopes_to_timestamp = [l.split(':')[1].strip() for l in lines if l.startswith("ts_at_in_axn")]
    #self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = axn_scopes_to_timestamp

    if self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMS']:
      atoms_to_timestamp = [l.split(':')[1].strip() for l in lines if l.startswith("ts_at")]
    else:
      atoms_to_timestamp = []
    self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_TO_TIMESTAMP'] = atoms_to_timestamp
    self.logger.info('atoms_to_timestamp: %s' % atoms_to_timestamp)

    if self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS']:
      atoms_to_timestamp_internally = [l.split(':')[1].strip() for l in lines if l.startswith("ts_atii")]
    else:
      atoms_to_timestamp_internally = []
    self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = atoms_to_timestamp_internally
    self.logger.info('atoms_to_timestamp_internally: %s' % atoms_to_timestamp_internally)

    if atoms_to_debug or atoms_to_identify or atoms_to_timestamp_internally:
      GEN_BLOCK_DEBUG = True
    else:
      GEN_BLOCK_DEBUG = False

    confirm(self.IS_INTERACTIVE)

    return atoms_to_debug, atoms_to_identify, self.TIMESTAMP_CONFIG, GEN_BLOCK_DEBUG


  def whether_timestamp(self, primitive, prname="", prid=-1):
    if primitive == 'st':
      enabled = self.TIMESTAMP_CONFIG['TIMESTAMP_STATES']
      valid_set = self.TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP']

    elif primitive == 'ax':
      enabled = self.TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS']
      valid_set = self.TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS_TO_TIMESTAMP']

    elif primitive == 'at': 
      enabled = self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMS']
      valid_set = self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_TO_TIMESTAMP']

    elif primitive == 'ati': 
      enabled = not self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only']

    elif primitive == 'atii': 
      enabled = self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS']
      valid_set = self.TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP']

    else:
      self.logger.error('whether_timestamp: bad primitive')
      sys.exit(1)

    if not prname:
      return enabled
    else:
      if enabled:
	#if prname in valid_set or "*" in valid_set:
	#  return True
	for pattern in valid_set:
	  if re.match(pattern, prname):
	    self.logger.debug('whether_timestamp: primitive=%s, prname=%s, True' \
		  % (primitive, prname))
	    return True
	self.logger.debug('whether_timestamp: primitive=%s, prname=%s, False' \
	      % (primitive, prname))

    return False

  def get_atoms_to_optimize(self, atoms_to_optimize_file, HAVE_L1PSRAM, HAVE_L1DSRAM):
    f = open(atoms_to_optimize_file, 'r')
    atoms_to_optimize = [x.strip() for x in f.readlines()]
    f.close()

    atoms_to_optimize = [x for x in atoms_to_optimize if x != '' and not x.startswith('#')]

    if HAVE_L1PSRAM:
      atoms_to_optimize_for_code = atoms_to_optimize
    else:
      atoms_to_optimize_for_code  = []

    if HAVE_L1DSRAM:
      atoms_to_optimize_for_data = atoms_to_optimize
    else:
      atoms_to_optimize_for_data = []

    self.logger.info("atoms_to_optimize_for_code: %s" % atoms_to_optimize_for_code)
    self.logger.info("atoms_to_optimize_for_data: %s" % atoms_to_optimize_for_data)
    confirm(self.IS_INTERACTIVE)
    return atoms_to_optimize_for_code, atoms_to_optimize_for_data

  def __init__(self, logger, IS_INTERACTIVE):
    self.logger = logger
    self.IS_INTERACTIVE = IS_INTERACTIVE
    self.TIMESTAMP_CONFIG = {}
