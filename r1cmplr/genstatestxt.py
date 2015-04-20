#!/usr/bin/python
'''
Atomix project, genstatestxt.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

####################
def printAxns(app, f):
    axnlist = {}
    atoms = app.getAtoms();
    for atom in atoms:
        if(atom.atomType is "decision_t"):
            continue
        coreid = atom.coreID
        stateid = atom.stateID
        if((stateid, coreid) not in axnlist.keys()):
            axnlist[(stateid, coreid)] = []
        axnlist[(stateid, coreid)].append(atom.atomName)
    
    axnlistkeys = axnlist.keys()
    axnlistkeys.sort() # Check if this is even possible 

    for key in axnlistkeys:
        f.write("\naction: axn_%d_%d: "%(key[0], key[1])),
        val = axnlist[key]
        # val is the  list of atoms
        for atName in val:
            f.write("%s;"%atName),
    f.write("\n")
    f.write("#\n")
    return axnlist

#####################

def printStates(app, f):
    states = app.getStates()
    for state in states:
        f.write("state : %s\n"%state.stateName)
    f.write("#\n")

#######################
def printStateps(app, axnlist, hwMgr, f):
    states = app.getStates()
    numCores = hwMgr.getNumCores()
    
    for state in states:
        ruleBlock = state.getRule()
        ruleAtom = app.atomList[(ruleBlock.IDval, state.IDval)];
        for coreid in range(1, numCores+1):
            if((state.IDval, coreid) in axnlist.keys()):
                f.write("statep: %s: %d: axn_%d_%d: "%(state.stateName, coreid, state.IDval, coreid)),
                if(ruleAtom.coreID == coreid):
                    f.write("%s;"%ruleAtom.atomName),
                else:
                    f.write("\t"),
                f.write("cp_%d_%d;" %(state.IDval, coreid)),
                f.write("tr_%d_%d;\n" %(state.IDval, coreid)) 
            else:
                f.write("statep: %s: %d: noaxn :\t\t tr_%d_%d;\n" %(state.stateName, coreid, state.IDval, coreid))
    f.write("#\n")

def printInitState(app, f):
    states = app.getStates()
    for state in states:
        if(state.isInitState == True):
            print("\n")

def genStatesTxt(app, hwMgr):
    f = open("states.txt","w")
    axnlist = printAxns(app, f)
    printStates(app, f)
    printStateps(app, axnlist, hwMgr, f)
    printInitState(app, f)
    f.close()

    
