# atomix
Atomix programming framework release (under development)

## Build instructions ##

### Setting up required tools ###

Install these packages on linux: 

```
# Build system support
# sudo apt-get install autoconf automake libtool git make python python-networkx realpath python-matplotlib python-pip vim

# python sqlite3 upgrade
# sudo pip uninstall pysqlite; sudo apt-get install libsqlite3-dev; sudo pip install pysqlite

# TI's C compiler
# cl6x #install TI Code Generation Tools (CGT) (ex. v7.2.4, or v7.3.3)
```

Note: If you installed Code Composer Studio, CGT may have already been installed. If you can find the compiler command cl6x on your path (or in the CCS install folder hierarchy), you already have CGT. You can use that corresponding CGT root for rest of the steps and do not need to install CGT again.

### Get the source ###

clone atomix with git: 

$ cd ~/workspace; git clone https://bitbucket.org/manubansal/atomix.git (or similar)


clone tiextlibs: 

$ cd ~/workspace; git clone https://github.com/manubansal/tiextlibs


### Set up environment variables ###

Setup paths in .bashrc (pay special attention to slash, backslash and root/drive letter conventions in setting those paths - you want to maintain these conventions):
note: when changing paths don't use home shortcut (e.g. ~/) but full paths (e.g. /home/manub/)

```
#!/bin/bash

########################################
#### for atomix
########################################

#---- USER SETUP START -----
#modify according to your workspace path
export ATOMIX_ROOT=/home/manub/workspace/atomix
export TIEXTLIBS_ROOT=/home/manub/workspace/tiextlibs

#modify according to your CCS path and CGT version
export CCS_ROOT=/opt/ti/ccsv6
export CG_TOOL_ROOT=$CCS_ROOT/tools/compiler/c6000_7.4.1	#cl6x should be in $CG_TOOL_ROOT/bin

#export CSL_ROOT="$TIEXTLIBS_ROOT/csl_tci6616_1.0.0.21/packages"           #for 6618
export CSL_ROOT="$TIEXTLIBS_ROOT/pdk_C6670_1_1_2_5/packages"             #for 6670
#---- USER SETUP END -----

alias cygpath="_cygpath"
export CCS_ROOT_WIN_STYLE=$CCS_ROOT
export CG_TOOL_ROOT_WIN=$CG_TOOL_ROOT
export ATOMIX_ROOT_LINUX_STYLE=$ATOMIX_ROOT
export ATOMIX_INSTALL_DIR=$ATOMIX_ROOT/build
export PDK_ROOT=$CSL_ROOT
export BUILD_UTILS_ROOT=$ATOMIX_ROOT_LINUX_STYLE/r2cmplr
export PATH=$BUILD_UTILS_ROOT:"$CG_TOOL_ROOT/bin":$PATH
export PATH=$PATH:$ATOMIX_ROOT_LINUX_STYLE/tracescripts
export DEBUGSERVER_ROOT="$CCS_ROOT/ccs_base/DebugServer"
#------------
########################################
```

do some one-time bootstrapping:

```
$ cd <path_to>/tiextlibs
$ make install
```



### Build the source ###

In the atomix folder, do the following steps:

Initialize configuration scripts only once:

$ cd atomix; autoreconf --install


Create the directory in which the project will be built:

$ cd atomix; mkdir build


Set up the build configuration:

$ cd ~/atomix/build; ../_configure


Build libraries:

$ cd ~/atomix/build; make




### Configuration parameters (for custom build) ###

Example custom configuration:

```
 ../_configure tsconf=none debug=2 memconf=memconf1 inptracemode=lc snrber=yes  #representative settings -- pick your own values for flags
```

#### Target board specification

host: evm6474 | [nyquist] | evm6670 | 6638k2k | 66ak2h14

Pass an option like `host=nyquist` in the _configure command line.

#### Memory configuration

memconf: TBD ([memconf1])

#### Timestamp configuration

tsconf: TBD

#### Debug level configuration

debug: [0] | 1 | 2 | 3 | 4  Higher value prints out more, consumes more program memory, runs slower.

#### Ethernet driver build

By default, an ethernet driver included as part of OSL (support library) is
built as a library.  This is needed for evm6474, nyquist, and evm6670. Some
newer boards like 6638k2k and 66ak2h14 do not need this ethernet driver library
as the assisting PDK already includes one. For those boards, trying to build
the included driver will cause the build to fail. The ethernet driver library
will need to be disabled from atomix build using a command line flag of
`--disable-osleth` or, equivalently, `--enable-osleth=no`.


### Set up VIM syntax highlighting (optional) ###

Before looking at the examples, you may want to set up VIM syntax highlighting. It makes the code more readable.

1. atomix/dotvim/filetype.vim should be placed in ~/.vim/
2. atomix/dotvim/or.vim should be placed in ~/.vim/syntax/

Steps 1 and 2 happen automatically with 

$ cd ~/atomix/build; make install-vim-syntax


They can be undone using

$ cd ~/atomix/build; make clean-local


Atomix source files should be named with extension .or for syntax highlighting

---

## Hello World! ##

Clone axhello from https://github.com/manubansal/axhello.git, then follow the contained instructions to build helloworld applications.

---

## Run and Debug ##


This section describes how to use Code Composer Studio to run and debug OpenRadio binaries using the built in TI debugger.

### Setting up CCS for EVM6616/EVM6618 ###

On Windows:
[These instructions are based on CCSv5.2.0.00069]
Assuming you set up CCSv5 in the default path, to be able to connect and emulate with the 6616 EVM, you need to copy nyquist_tms320c66xx.xml into C:/ti/ccsv5/ccs_base/common/targetdb/devices/

On Linux:
Set up the CCS_ROOT environment variable in your .bashrc to something like:
export CCS_ROOT=/opt/ti/ccsv5
From the atomix/build folder, run the following command:
$ make install-data-local
This will copy the nyquist_tms320c66xx_linux.xml to the right location, for which the linux target configuration file for the 6618 board (targetconfs/evmbe6618_usb_daisy_chain_linux_selfcontained.ccxml) has been set up.

### Importing atomix project into CCS ###
[These instructions are based on CCSv5].

We assume that you have already checked out atomix code and built it using cygwin make tools as described in OpenRadio build manual. Next, you want to import atomix into Code Composer Studio as a CCS project. Do File → Import → Code Composer Studio/Existing CCS/CCE Eclipse Projects → Next → Select search-directory → Browse to the atomix folder. atomix eclipse project should be discovered and ready to import by clicking the check box. Make sure “Copy projects into workspace” box is unchecked because you want to use the same source/build tree as you use with the cygwin make tools, not a copy of it. Finally, click Finish.

### Launching the debugger ###
After importing the project, you want to pick a target configuration with which to launch the debugger. The target configuration describes the hardware on which your binary will execute. It could describe a simulator or an emulator that would connect to a real platform EVM. Pick View → Target Configurations. This will open up a file tree in CCS. Go to Projects → atomix → targetconfs. You should see a bunch of .ccxml files listed. These are different target configurations.

Right click a target configuration (say, EVMTCI6616_USB_Daisy_Chain_manu.ccxml)
[Windows] EVMTCI6616 USB Daisy Chain.ccxml or evmbe6618_usb_daisy_chain_linux_selfcontained.ccxml [Linux] and pick Launch Selected Configuration. If all goes well (no red lines in the output console, no errors loading gel files etc.), group all of the cores into a group by selecting them then right-click->Group Cores. Now you should be able to pick Run → Connect Target. Once you do that, you should be able to able to pick Run → Load → Load Program. Once doing that, browse to the proper .out (binary) file in the atomix/build folder tree and load that.

### Executing and debugging binaries ###
If you could successfully launch the debugger and load your binary, you should see code association too, that is, CCS editor will start showing location of current program counter in the source. You will be able run the code by clicking the “Play” button on the Debug pane. You will also be able to set breakpoints by double clicking on the left margin in the editor. And finally, if you have prints in your code, you will get output in the console.

### Things to avoid ###
Note that this target configuration launch sequence of steps is picked so that CCS does not automatically try to build the code. This is important because we don’t use the CCS build system and any attempt to build the code from within CCS will likely fail or produce wrong binaries. The usual procedure of picking a binary from the list of binaries in the project explorer (if visible) includes the build step and there does not allow launching the debugger. Don’t do that.

In case you ended up firing CCS build process and therefore ended up with a Debug or a Release folder, make sure you don’t commit it into the repository. You can safely remove and following the instructions above to run and debug even if you ended up with CCS trying to build.

---



