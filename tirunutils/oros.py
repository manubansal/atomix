'''
Atomix project, oros.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from java.lang import *
from java.util import *
from com.ti.debug.engine.scripting import *
from com.ti.ccstudio.scripting.environment import *
#import argparse
import optparse

def pause(s):
  print s, 'Print any key to continue...'
  raw_input()
 
def create_script():
  # Create our scripting environment object - which is the main entry point into any script and
  # the factory for creating other Scriptable Servers and Sessions
  script = ScriptingEnvironment.instance()
   
  # Create a log file in the current directory to log script execution
  script.traceBegin("dss_py.xml", "DefaultStylesheet.xsl")
   
  # Set our TimeOut
  #script.setScriptTimeout(15000)
   
  # Log everything
  script.traceSetConsoleLevel(TraceLevel.ALL)
  script.traceSetFileLevel(TraceLevel.ALL)

  return script

#def startup(script):
#  # Get the Debug Server and start a Debug Session
#  debugServer = script.getServer("DebugServer.1")
#  debugServer.setConfig("tisim_c64xple.ccxml");
#   
#  debugSession = debugServer.openSession()
#   
#  # Load program
#  debugSession.memory.loadProgram("modem.out")
#   
#  # Set a breakpoint at "main"
#  main = debugSession.symbol.getAddress("main")
#  bp1 = debugSession.breakpoint.add(main)
#   
#  # Set another breakpoint
#  address = debugSession.symbol.getAddress("ReadNextData")
#  bp2 = debugSession.breakpoint.add(address)
#   
#  # Restart our Target
#  debugSession.target.restart()
#   
#  # Run if already not automatically halted at main.  Should halt at first BP
#  if debugSession.expression.evaluate("PC") != main:
#	  debugSession.target.run()
#   
#  # Using an expression - get the current value of the PC
#  nPC = debugSession.expression.evaluate("PC")
#   
#  # Verify we halted at the correct address.  Use hex() to convert the 
#  # result to a hex string when logging messages
#  if nPC == main:
#	  script.traceWrite("SUCCESS: Halted at correct location")
#  else:
#	  script.traceWrite("FAIL: Expected halt at " + hex(address) + ", actually halted at " + hex(nPC))
#	  script.traceSetConsoleLevel(TraceLevel.INFO)
#	  script.traceWrite("TEST FAILED!")
#	  script.traceEnd()
#	  System.exit(1);
#   
#  # Run again.  Should halt at our breakpoint
#  debugSession.target.run()
#   
#  #  Using an expression - get the current value of the PC
#  nPC = debugSession.expression.evaluate("PC")
#   
#  # Verify we halted at the correct address.
#  if nPC == address:
#	  script.traceWrite("SUCCESS: Halted at correct location")
#  else:
#	  script.traceWrite("FAIL: Expected halt at " + hex(address) + ", actually halted at " + hex(nPC))
#	  script.traceSetConsoleLevel(TraceLevel.INFO)
#	  script.traceWrite("TEST FAILED!")
#	  script.traceEnd()
#	  System.exit(1);
#   
#  # All done
#  debugSession.terminate()
#  debugServer.stop()

def startup_debugServer(script, options):
  # Start up CCS
  ccsServer = None
  ccsSession = None
  if ((not options.cleanup) and options.gui):
    #pause("Press enter to launch CCS...")
    ccsServer = script.getServer("CCSServer.1")
    ccsSession = ccsServer.openSession(".*")

  # Start up the debugger with the C64 simulator; it will start up the session in CCS
  debugServer = script.getServer("DebugServer.1")
  #debugServer.setConfig("../C64/tisim_c64xple.ccxml")
  #debugServer.setConfig("../targetconfs/evmbe6670_28K.ccxml")
  debugServer.setConfig(options.tcf); #"../targetconfs/evmbe6670_28K.ccxml")

  return (ccsServer, ccsSession, debugServer)

def startup_debugSessions(debugServer, SESSION_IDS):
  #debugSession = debugServer.openSession(".*")

  #pause("Press enter to open debug sessions...")

  pause("Press enter to open debug session 1")
  debugSession0 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_00")

  ##pause("Press enter to open debug session 2")
  ##debugSession1 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_1")

  ##pause("Press enter to open debug session 3")
  ##debugSession2 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_2")

  ##pause("Press enter to open debug session 4")
  ##debugSession3 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_3")

  debugSessions = []
  ##for i, session_id in enumerate(SESSION_IDS):
  ##  pause("Press enter to open debug session " + str(i))
  ##  print "Opening", session_id
  ##  debugSession = debugServer.openSession(session_id)
  ##  debugSessions.append(debugSession)

  return debugSessions


def connectAndRun():
  pause("Press enter to connect to target...")

  debugSession0.target.connect()
  debugSession1.target.connect()
  debugSession2.target.connect()
  debugSession3.target.connect()

  # Load a program
  #var isaName = "../C64"
  ##script.setScriptTimeout(15000);
  #debugSession.memory.loadProgram(isaName + "/modem/Debug/modem.out")
  #debugSession0.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  #debugSession1.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  #debugSession2.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  #debugSession3.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  print("++++++ loading " + options.prog)
  script.traceWrite("++++++ loading " + options.prog)
  pause("Press enter to load program...")
  debugSession0.memory.loadProgram(options.prog);
  debugSession1.memory.loadProgram(options.prog);
  debugSession2.memory.loadProgram(options.prog);
  debugSession3.memory.loadProgram(options.prog);


  pause("Press enter to run program...")
  debugSession0.target.runAsynch() 
  debugSession1.target.runAsynch()
  debugSession2.target.runAsynch()
  debugSession3.target.runAsynch()

def cleanup():
  cleanup_debug()
  cleanup_ccs()
  cleanup_script(script)

def cleanup_debug(debugServer):
  pause("Press enter to terminate sessions...")
  # terminate the debugger
  ##debugSession3.terminate()
  ##debugSession2.terminate()
  ##debugSession1.terminate()
  ##debugSession0.terminate()

  debugServer.stop()


def cleanup_ccs(ccsServer, ccsSession, options):
  # Terminate CCS
  if ((not options.cleanup) and options.gui):
    pause("Press enter to terminate CCS...")
    ccsSession.terminate()
    ccsServer.stop()

def cleanup_script(script):
  # stop the Logging
  script.traceSetConsoleLevel(TraceLevel.INFO)
  script.traceWrite("TEST SUCCEEDED!")
  script.traceEnd()

def main():
    # my code here
    #parser = argparse.ArgumentParser(description='Process some integers.')
    #parser.add_argument("-c", "--tcf", 
    #	help="target configuration file", action="store")
    #parser.add_argument("-p", "--prog",
    #	help="program to load", action="store")
    #parser.add_argument("-g", "--gui", 
    #	help="attach and launch CCS GUI", action="store_true")
    #parser.add_argument("-C", "--cleanup",
    #	help="cleanup debug sessions", action="store_true")

    #options = parser.parse_args()
    #print options

    parser = optparse.OptionParser()
    parser.add_option("-c", "--tcf", 
    	help="target configuration file", action="store")
    parser.add_option("-p", "--prog",
    	help="program to load", action="store")
    parser.add_option("-g", "--gui", default=False,
    	help="attach and launch CCS GUI", action="store_true")
    parser.add_option("-C", "--cleanup", default=False,
    	help="cleanup debug sessions", action="store_true")

    (options, args) = parser.parse_args()
    print options
    print args

    SESSION_PREFIX = "Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_0"
    #SESSION_PREFIX = "Blackhawk XDS560v2-USB .*"
    SESSION_IDS = [SESSION_PREFIX + str(i) for i in range(4)]
    print SESSION_IDS

    script = create_script()
    (ccsServer, ccsSession, debugServer) = startup_debugServer(script, options)

    #cleanup_debug(debugServer)
    #cleanup_script(script)
    #return

    debugSessions = startup_debugSessions(debugServer, SESSION_IDS)

    cleanup_debug(debugServer)
    #cleanup_ccs(ccsServer, ccsSession, options)
    cleanup_script(script)

    return

    if (options.cleanup):
      pass
    else:
      connectAndRun();

    cleanup()

if __name__ == "__main__":
    main()

