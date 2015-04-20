'''
Atomix project, jsutil.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import time

def pss_b(jsproc, cmd):
  print
  print '>>>>>>>>>>'
  print cmd
  print '<<<<<<<<<<'
  print
  jsproc.stdin.write(cmd)
  fetch_jsprompt(jsproc)
  time.sleep(0.2)

def fetch_jsprompt(jsproc):
  out = ''
  while not out.endswith('js>'):
    out = out + jsproc.stderr.read(1)
  out = out[:-3]
  if len(out) > 0:
    print 'ERR:<', out, '>'

#//http://4thmouse.com/index.php/2008/03/11/embedding-rhino-part-i/

#//--------------------------------- main script ---------------------------

#// Example: 
#// 1. Launches CCS GUI
#// 2. Launches the debugger for the C64 simulator
#// 3. Load a program
#// 4. Terminates the debugger, CCS, etc.

def code_import_packages():
  code = '''
  // Import the DSS packages into our namespace to save on typing
  importPackage(Packages.com.ti.debug.engine.scripting)
  importPackage(Packages.com.ti.ccstudio.scripting.environment)
  importPackage(Packages.java.lang)
  importPackage(Packages.java.io)
  '''
  return code


def code_create_scripting_env():
  code = '''
  // Create our scripting environment object - which is the main entry point into any script and
  // the factory for creating other Scriptable ervers and Sessions
  var script = ScriptingEnvironment.instance()

  script.traceBegin("CCSSession.xml", "DefaultStylesheet.xsl")

  // Log everything
  script.traceSetConsoleLevel(TraceLevel.ALL)
  script.traceSetFileLevel(TraceLevel.ALL)
  '''
  return code


def code_start_ccs():
  code = '''
  // Start up CCS
  ccsServer = script.getServer("CCSServer.1")
  ccsSession = ccsServer.openSession(".*")
  '''
  return code

def code_start_debugger(tcf):

  #//debugServer.setConfig("../C64/tisim_c64xple.ccxml")
  #//debugServer.setConfig("../targetconfs/evmbe6670_28K.ccxml")
  #//debugServer.setConfig(options.tcf); //"../targetconfs/evmbe6670_28K.ccxml")

  code = '''
  debugServer = script.getServer("DebugServer.1");
  '''

  code += 'debugServer.setConfig(%s);\n' % tcf

  return code

def code_start_debug_session(session_id, session_name):
  code = '%s = debugServer.openSession(%s)\n' % (session_id, session_name)
  return code

def code_connect_debug_session(session_id):
  code = '%s.target.connect()\n' % session_id
  return code

def code_load_program(session_id, prog):
#  // Load a program
#  //var isaName = "../C64"
#  ////script.setScriptTimeout(15000);
#  //debugSession.memory.loadProgram(isaName + "/modem/Debug/modem.out")
#  //debugSession0.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
#  //debugSession1.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
#  //debugSession2.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
#  //debugSession3.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
#  print("++++++ loading " + options.prog)
#  script.traceWrite("++++++ loading " + options.prog)
#  pause("Press enter to load program...")
#  debugSession0.memory.loadProgram(options.prog);
#  debugSession1.memory.loadProgram(options.prog);
#  debugSession2.memory.loadProgram(options.prog);
#  debugSession3.memory.loadProgram(options.prog);
   code = '%s.memory.loadProgram(%s)\n' % (session_id, prog)
   return code

def code_run_target(session_id):
#
#
#  pause("Press enter to run program...")
#  debugSession0.target.runAsynch() 
#  debugSession1.target.runAsynch()
#  debugSession2.target.runAsynch()
#  debugSession3.target.runAsynch()
#}
   code = '%s.target.runAsynch()\n' % session_id
   return code

def code_halt_target(session_id):
   code = '%s.target.halt()\n' % session_id
   return code

def code_restart_target(session_id):
   code = '%s.target.restart()\n' % session_id
   return code

def code_terminate_debug_session(session_id):
  #//pause("Press enter to terminate sessions...")
  #// terminate the debugger

  #debugSession3.terminate()
  #debugSession2.terminate()
  #debugSession1.terminate()
  #debugSession0.terminate()

  code = '%s.terminate()\n' % session_id
  return code

def code_stop_ccs():
  code = '''
  // Terminate CCS
  ccsSession.terminate()
  ccsServer.stop()
  '''
  return code

def code_stop_debug_server():
  code = 'debugServer.stop()\n'
  return code

def code_stop_scripting_env():
  code = '''
  // stop the Logging
  script.traceSetConsoleLevel(TraceLevel.INFO)
  script.traceWrite("TEST SUCCEEDED!")
  script.traceEnd()
  '''
  return code

