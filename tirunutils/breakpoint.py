'''
Atomix project, breakpoint.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from java.lang import *
from java.util import *
from com.ti.debug.engine.scripting import *
from com.ti.ccstudio.scripting.environment import *
 
# Create our scripting environment object - which is the main entry point into any script and
# the factory for creating other Scriptable Servers and Sessions
script = ScriptingEnvironment.instance()
 
# Create a log file in the current directory to log script execution
script.traceBegin("dss_py.xml", "DefaultStylesheet.xsl")
 
# Set our TimeOut
script.setScriptTimeout(15000)
 
# Log everything
script.traceSetConsoleLevel(TraceLevel.ALL)
script.traceSetFileLevel(TraceLevel.ALL)
 
# Get the Debug Server and start a Debug Session
debugServer = script.getServer("DebugServer.1")
debugServer.setConfig("tisim_c64xple.ccxml");
 
debugSession = debugServer.openSession()
 
# Load program
debugSession.memory.loadProgram("modem.out")
 
# Set a breakpoint at "main"
main = debugSession.symbol.getAddress("main")
bp1 = debugSession.breakpoint.add(main)
 
# Set another breakpoint
address = debugSession.symbol.getAddress("ReadNextData")
bp2 = debugSession.breakpoint.add(address)
 
# Restart our Target
debugSession.target.restart()
 
# Run if already not automatically halted at main.  Should halt at first BP
if debugSession.expression.evaluate("PC") != main:
	debugSession.target.run()
 
# Using an expression - get the current value of the PC
nPC = debugSession.expression.evaluate("PC")
 
# Verify we halted at the correct address.  Use hex() to convert the 
# result to a hex string when logging messages
if nPC == main:
	script.traceWrite("SUCCESS: Halted at correct location")
else:
	script.traceWrite("FAIL: Expected halt at " + hex(address) + ", actually halted at " + hex(nPC))
	script.traceSetConsoleLevel(TraceLevel.INFO)
	script.traceWrite("TEST FAILED!")
	script.traceEnd()
	System.exit(1);
 
# Run again.  Should halt at our breakpoint
debugSession.target.run()
 
#  Using an expression - get the current value of the PC
nPC = debugSession.expression.evaluate("PC")
 
# Verify we halted at the correct address.
if nPC == address:
	script.traceWrite("SUCCESS: Halted at correct location")
else:
	script.traceWrite("FAIL: Expected halt at " + hex(address) + ", actually halted at " + hex(nPC))
	script.traceSetConsoleLevel(TraceLevel.INFO)
	script.traceWrite("TEST FAILED!")
	script.traceEnd()
	System.exit(1);
 
# All done
debugSession.terminate()
debugServer.stop()
