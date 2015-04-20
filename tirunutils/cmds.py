'''
Atomix project, cmds.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sys
import imp
import time

jsutil = imp.load_source('jsutil','jsutil.py')

def cmd_help(jsproc, options, args):
  selfmodule = sys.modules[__name__]
  CMDS = [cmd_name.split('cmd_')[1] for cmd_name in dir(selfmodule) if cmd_name.startswith('cmd_')]
  print 'Known commands:', CMDS

  return 0

def cmd_bye(jsproc, options, args):
  pass

  return 0

def cmd_prog(jsproc, options, args):
  if len(args) > 0:
    prog = args[0]
    print 'Setting program to', prog
  else:
    prog = options.prog

  print 'program image:', prog

  return 0

def cmd_tcf(jsproc, options, args):
  print options.tcf

  return 0

def cmd_lscpu(jsproc, options, args):
  for cpu in options.debugSessions:
    print cpu, options.debugSessions[cpu]

  return 0

def cmd_load(jsproc, options, args):
  if len(args) > 0:
    session_id = args[0]
  else:
    print 'ERROR: need a session_id to identify load target'
    return 1

  prog = options.prog

  print 'loading program to', session_id
  print 'program image:', prog

  jsutil.pss_b(jsproc, jsutil.code_load_program(session_id, prog))
  return 0

def cmd_run(jsproc, options, args):
  if len(args) > 0:
    session_id = args[0]
  else:
    print 'ERROR: need a session_id to identify run target'
    return 1

  jsutil.pss_b(jsproc, jsutil.code_run_target(session_id))
  return 0

def cmd_halt(jsproc, options, args):
  if len(args) > 0:
    session_id = args[0]
  else:
    print 'ERROR: need a session_id to identify halt target'
    return 1

  jsutil.pss_b(jsproc, jsutil.code_halt_target(session_id))
  return 0

def cmd_restart(jsproc, options, args):
  if len(args) > 0:
    session_id = args[0]
  else:
    print 'ERROR: need a session_id to identify restart target'
    return 1

  jsutil.pss_b(jsproc, jsutil.code_restart_target(session_id))
  return 0


def cmd_evm_startup(jsproc, options, args):
  print 'Initiating evm startup'

  print 'Importing packages'
  jsutil.pss_b(jsproc, jsutil.code_import_packages())

  print 'Creating scripting environment'
  jsutil.pss_b(jsproc, jsutil.code_create_scripting_env())

  if ((not options.cleanup) and options.gui):
    print 'Starting CCS'
    jsutil.pss_b(jsproc, jsutil.code_start_ccs())
    print 'Waiting 5 secs to let CCS start...'
    time.sleep(5)

  print 'Starting debug server'
  jsutil.pss_b(jsproc, jsutil.code_start_debugger(options.tcf))
  time.sleep(10)

  print 'Starting debug sessions'
  for session_id in options.debugSessions:
    session_name = options.debugSessions[session_id]
    jsutil.pss_b(jsproc, jsutil.code_start_debug_session(session_id, session_name))
    time.sleep(12)
    
  print 'Connecting debug sessions'
  for session_id in options.debugSessions:
    jsutil.pss_b(jsproc, jsutil.code_connect_debug_session(session_id))
    time.sleep(12)

  print 'Finished evm startup'

  return 0

def cmd_evm_shutdown(jsproc, options, args):
  print 'Initiating evm shutdown'

  print 'Stopping debug sessions'
  for session_id in options.debugSessions:
    jsutil.pss_b(jsproc, jsutil.code_terminate_debug_session(session_id))
    time.sleep(10)

  if ((not options.cleanup) and options.gui):
    print 'Stopping CCS'
    jsutil.pss_b(jsproc, jsutil.code_stop_ccs())
    time.sleep(10)

  print 'Stopping debug server'
  jsutil.pss_b(jsproc, jsutil.code_stop_debug_server())
  time.sleep(5)

  print 'Stopping scripting environment'
  jsutil.pss_b(jsproc, jsutil.code_stop_scripting_env())
  time.sleep(5)

  print 'Finished evm shutdown'

  return 0


def main():
  cmd_help()

if __name__ == "__main__":
  main()
