//Designed to look like the IE dictionary
function Dictionary(){
    var getmap;
    //using closure to provide a private member variable
    (function() {
        var _map = new java.util.HashMap();
        getmap = function(){
            return _map;
        };
    })();
 
    this.add = function(key, value){
        getmap().put(key, value);
    };
    this.get = function(key){
        return getmap().get(key);
    };
    this.exists = function(key){
        return getmap().keySet().contains(key);
    };
    this.remove = function(key){
        getmap().remove(key);
    };
    this.removeall = function(key){
        getmap().clear();
    };
}

//represents a single option definition
function Option(shortname, longname, argument, description, action){
    this.shortname = shortname;
    this.longname = longname;
    this.argument = argument;
    this.description = description;
    this.action = action;
    this.requires_argument = function(){
        return this.argument != null;
    };
}
 
//parser for parsing command line options
function OptionParser(help_message){
    var getshorttooptionmap,
        getlongtooptionmap;
 
    this.options = [];
    this.help_message = help_message;
 
    (function() {
        var shorttooptionmap = new Dictionary();
        var longtooptionmap = new Dictionary();
 
        getshorttooptionmap = function(){
            return shorttooptionmap;
        }
 
        getlongtooptionmap = function(){
            return longtooptionmap;
        }
    })();
 
    this.addoption = function(shortname, longname, 
                              argument, description, action){
        option = new Option(shortname, longname, argument, 
                            description, action);
        getshorttooptionmap().add(shortname, option);
        getlongtooptionmap().add(longname, option);
        this.options.push(option);
    }
 
    this.addoptionflag = function(shortname, longname, 
                                  description, action){
        this.addoption(shortname, longname, 
                       null, description, action);
    }
 
 
    this.parse = function(argv){
        var option;
        for(i = 0; i < argv.length; i++){
            if(argv[i].substr(0,2) == "--"){
                option = getlongtooptionmap().get(argv[i].substr(2));
            }else if(argv[i].substr(0,1) == "-"){
                option = getshorttooptionmap().get(argv[i].substr(1));
            }else{
                throw "Was expecting option. Got " + argv[i];
            }
 
            if(option == undefined){
                throw "Undefined option: " + argv[i];
            }
 
            if(option.requires_argument()){
                if(i == argv.length - 1){
                    throw "option " + argv[i] + " requires argument";
                }
                arg = argv[i + 1];
                i++;
                option.action(arg);
            }else{
                option.action();
            }
        }
    }
 
    this.help = function(){
        ret = "";
        ret = ret + help_message + "\n";
        for(i=0; i< this.options.length; i++){
            ret = ret + "\t-" + this.options[i].shortname;
            ret = ret + " --" + this.options[i].longname;
            if(this.options[i].argument != null){
                ret = ret + " [" + this.options[i].argument + "]";
            }
            ret = ret + " " + this.options[i].description + "\n";
        }
        return ret;
    }
}




function startup() {

  // Start up CCS
  if (!options.cleanup && options.gui) {
    //pause("Press enter to launch CCS...")
    ccsServer = script.getServer("CCSServer.1")
    ccsSession = ccsServer.openSession(".*")
  }

  // Start up the debugger with the C64 simulator; it will start up the session in CCS
  debugServer = script.getServer("DebugServer.1")
  //debugServer.setConfig("../C64/tisim_c64xple.ccxml")
  //debugServer.setConfig("../targetconfs/evmbe6670_28K.ccxml")
  debugServer.setConfig(options.tcf); //"../targetconfs/evmbe6670_28K.ccxml")
  //debugSession = debugServer.openSession(".*")

  //pause("Press enter to open debug sessions...")

  pause("Press enter to open debug session 1")
  debugSession0 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_00")

  pause("Press enter to open debug session 2")
  debugSession1 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_01")

  pause("Press enter to open debug session 3")
  debugSession2 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_02")

  pause("Press enter to open debug session 4")
  debugSession3 = debugServer.openSession("Blackhawk XDS560v2-USB System Trace Emulator_0/C66xx_03")
}


function connectAndRun() {
  pause("Press enter to connect to target...")

  debugSession0.target.connect()
  debugSession1.target.connect()
  debugSession2.target.connect()
  debugSession3.target.connect()

  // Load a program
  //var isaName = "../C64"
  ////script.setScriptTimeout(15000);
  //debugSession.memory.loadProgram(isaName + "/modem/Debug/modem.out")
  //debugSession0.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  //debugSession1.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  //debugSession2.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
  //debugSession3.memory.loadProgram("../build/app/wifirx54mc/app_wifirx54mc.out")
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
}


function cleanup() {
  pause("Press enter to terminate sessions...")
  // terminate the debugger
  debugSession3.terminate()
  debugSession2.terminate()
  debugSession1.terminate()
  debugSession0.terminate()

  debugServer.stop()

  // stop the Logging
  script.traceSetConsoleLevel(TraceLevel.INFO)
  script.traceWrite("TEST SUCCEEDED!")
  script.traceEnd()

  // Terminate CCS
  if (!options.cleanup && options.gui) {
    pause("Press enter to terminate CCS...")
    ccsSession.terminate()
    ccsServer.stop()
  }
}

//--------------------------------- options ---------------------------
optionparser = new OptionParser("oros.js [options]");
var options = {};
options.gui = false;
 
optionparser.addoption("c", "tcf", "TCF",
                           "target configuration file", function(arg){
    options.tcf = arg;
});

optionparser.addoption("p", "prog", "PROG",
                           "program to load", function(arg){
    options.prog = arg;
});

optionparser.addoptionflag("g", "gui", 
                           "attach and launch CCS GUI too", function(){
    options.gui = true;
});
optionparser.addoptionflag("h", "help", "print this message", function(){
    print(optionparser.help());
    java.lang.System.exit(0);
});

optionparser.addoptionflag("C", "cleanup", 
                           "cleanup debug sessions", function(){
    options.cleanup = true;
});
 
try{
    //optionparser.parse(argv);
    optionparser.parse(arguments);
}catch(e){
    print(e);
    print("\n");
    print(optionparser.help());
    java.lang.System.exit(1);
}
 
if(options.tcf == undefined){
    print("Must specify a target configuration file\n");
    print(optionparser.help());
    java.lang.System.exit(1);
}
else {
    print("TCF: " + options.tcf);
}

if(options.prog == undefined){
    print("Must specify a program to load\n");
    print(optionparser.help());
    java.lang.System.exit(1);
}
else {
    print("PROG: " + options.prog);
}
 

//--------------------------------- main script ---------------------------

// Example: 
// 1. Launches CCS GUI
// 2. Launches the debugger for the C64 simulator
// 3. Load a program
// 4. Terminates the debugger, CCS, etc.

// Import the DSS packages into our namespace to save on typing
importPackage(Packages.com.ti.debug.engine.scripting)
importPackage(Packages.com.ti.ccstudio.scripting.environment)
importPackage(Packages.java.lang)
importPackage(Packages.java.io)

lineReader = new BufferedReader(new InputStreamReader(System['in'], "UTF-8"));
function pause(msg) {
  print(msg);
  response = lineReader.readLine();
}


// Create our scripting environment object - which is the main entry point into any script and
// the factory for creating other Scriptable ervers and Sessions
var script = ScriptingEnvironment.instance()

script.traceBegin("CCSSession.xml", "DefaultStylesheet.xsl")

// Log everything
script.traceSetConsoleLevel(TraceLevel.ALL)
script.traceSetFileLevel(TraceLevel.ALL)

////script.traceWrite("process.argv: " + process.argv)
//print ("args")
//for (var i = 0; i < arguments.length; i++) {
//  print(arguments[i]);
//}

startup();

if (options.cleanup) {
}
else {
  connectAndRun();
}

cleanup();

