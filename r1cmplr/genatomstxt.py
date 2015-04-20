#!/usr/bin/python
'''
Atomix project, genatomstxt.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

from genR2 import *

def printKernelAtoms(numCores, atom_file):
    # What do we need to print to kernel atoms
    # Nothing - just the number of cores
    for i in range(0, numCores):
        atom_file.write("atom: kk%d: BlockKK: %d\n" %(i,i))

def printCopyBlocks(app, atom_file):
    appStates = app.getStates()
    for state in appStates:
        stateid = state.IDval
        ruleBlockID = state.getRule().IDval
        if ((ruleBlockID, stateid) in app.blockCore.keys()):
            coreid = app.blockCore[ruleBlockID, stateid]
        else:
            print("ERROR! Asking for coreid for %d %d - not found\n" %(ruleBlockID, stateid))
            coreid = -1
        atom_file.write("atom: cp_%d_%d\n" %(stateid, coreid))

def printTrBlocks(numCores, numStates, atom_file):
    # What do we need to print tr atoms
    # Just the number of blocks and cores
    for stateid in range(1, (numStates+1)):
        for coreid in range(0, numCores):
            atom_file.write("atom: tr_%d_%d: BlockTR: %d\n" %(stateid,coreid, coreid))

def printAtoms(all_atoms, atom_file):
    for at in all_atoms:
        c = at.coreID;
        state = at.stateID;
        name = at.block;
        atname = at.atomName;
            
        atom_file.write("atom: %s: %s: %d\n" %(atname, name, c))

def printWires(numCores, app, atom_file):
    # For now, let's just generate fifos
    # There's a fifo after every kernel block
    cpblocks = []
    fifos = []
    for i in range(0, numCores):
        #print("fifo: ff_kk%d:%d:sin:%d\n" %(i,def_size,i))
        ffname = "ff_kk"+str("%d"%i)
        atom_file.write("wire: kk%d:%s\n" %(i, ffname))
        fifos.append(ffname)
    # There's a fifo after every first atom
    # for each state. 
    for state in app.getStates():
        for block in app.getBlocks():
            if((block.IDval, state.IDval) in app.atomList.keys()):
                atom = app.atomList[(block.IDval, state.IDval)]
                atom_file.write("wire:%s" %(atom.atomName)), 
                (in_edge_bids, out_edge_bids) = state.getEdges(block.IDval)
                for e_bid in in_edge_bids:
                    e_atom = app.atomList[(e_bid, state.IDval)] 
                    # These are the atoms with with 'atom' has an incoming edge  
                    ffname = "ff_"+e_atom.atomName;
                    fifos.append(ffname)
                    atom_file.write(":%s" %ffname),
                if(len(out_edge_bids) > 0):
                    ffname = "ff_"+atom.atomName;
                    fifos.append(ffname),
                    atom_file.write(":%s"%ffname)
                    for e_bid in out_edge_bids:
                        e_atom = app.atomList[(e_bid, state.IDval)]
                        if(e_atom.coreID != atom.coreID):
                            cpblocks.append((atom, e_atom))
                atom_file.write("\n");
    
                # We have to take care of cp blocks now
                # cp blocks copy result of computation of a block on a core
                # and copy to fifos, which connect to transfer blocks on other
                # cores. So, there should be a cp + tr combination for every 
                # edge that is between two cores

                # TBD

                ###### NOTE: This is just to copy output of decision blocks ####
                if(len(out_edge_bids) < 1):
                    # No outgoing edges. 
                    atom_file.write("wire:cp_%d_%d: ff_%s" %(state.IDval,atom.coreID,atom.atomName)),  
                    for coreid in range(0, numCores):
                        ffname = "ff_cp_"+"%d_%d_%d"%(state.IDval, atom.coreID, coreid)
                        fifos.append(ffname)
                        atom_file.write(",%s"%ffname),
                    atom_file.write("\n")


    for state in app.getStates():
#        for block in app.getBlocks():
#            if((block.IDval, state.IDval) in app.atomList.keys()):
        for coreid in range(0, numCores):
            atom_file.write("wire: tr_%d_%d:ff_cp_%d_%d_%d:ff_kk%d\n"%(state.IDval, coreid, state.IDval,coreid,coreid, coreid))
    
    atom_file.write("conf:")    

    print("printing all fifos now....")
    for f in fifos:
        print("fifo:%s\n"%f)
    return fifos

def printfifos(fifos, fifo_file):
    for f in fifos:
        fifo_file.write("fifo:%s\n"%f)
    
#####################

def makeR2Graph(app, hwMgr):

    # app has an atomList. We will copy it here
    # we will create kernel atoms and add appropriate fifos to them
    
    R2Graph = r2Graph(app, hwMgr)
    R2Graph.createKernelAtoms()
    #R2Graph.createTransferAtoms()
    #R2Graph.createCopyAtoms()

    R2Graph.addWires()

    R2Graph.printGraph()

    f1 = open("atoms_copy.txt", "w")
    f2 = open("fifos_copy.txt", "w")
    R2Graph.printAtoms(f1)
    R2Graph.printWires(f1, f2)
    R2Graph.printAtomConf(f1)
    R2Graph.printFifoConf(f2)
    f1.close()
    f2.close()
    
     
#    for i in range(0, numCores):
#        ffname = "ff_kk"+str("%d"%i)
#        atom_file.write("wire: kk%d:%s\n" %(i, ffname))
#        fifos.append(ffname)
    # There's a fifo after every first atom
    # for each state. 
#    for state in app.getStates():
#        for block in app.getBlocks():
#            if((block.IDval, state.IDval) in app.atomList.keys()):
#                atom = app.atomList[(block.IDval, state.IDval)]
#                atom_file.write("wire:%s" %(atom.atomName)), 
#                (in_edge_bids, out_edge_bids) = state.getEdges(block.IDval)
#                for e_bid in in_edge_bids:
#                    e_atom = app.atomList[(e_bid, state.IDval)] 
#                    # These are the atoms with with 'atom' has an incoming edge  
#                    ffname = "ff_"+e_atom.atomName;
#                    fifos.append(ffname)
#                    atom_file.write(":%s" %ffname),
#                if(len(out_edge_bids) > 0):
#                    ffname = "ff_"+atom.atomName;
#                    fifos.append(ffname),
#                    atom_file.write(":%s"%ffname)
#                    for e_bid in out_edge_bids:
#                        e_atom = app.atomList[(e_bid, state.IDval)]
#                        if(e_atom.coreID != atom.coreID):
#                           cpblocks.append((atom, e_atom))
#                atom_file.write("\n");
    
                # We have to take care of cp blocks now
                # cp blocks copy result of computation of a block on a core
                # and copy to fifos, which connect to transfer blocks on other
                # cores. So, there should be a cp + tr combination for every 
                # edge that is between two cores

                # TBD

                ###### NOTE: This is just to copy output of decision blocks ####
#                if(len(out_edge_bids) < 1):
#                    # No outgoing edges. 
#                    atom_file.write("wire:cp_%d_%d: ff_%s" %(state.IDval,atom.coreID,atom.atomName)),  
#                    for coreid in range(0, numCores):
#                        ffname = "ff_cp_"+"%d_%d_%d"%(state.IDval, atom.coreID, coreid)
#                        fifos.append(ffname)
#                        atom_file.write(",%s"%ffname),
#                    atom_file.write("\n")


#   for state in app.getStates():
#        for block in app.getBlocks():
#            if((block.IDval, state.IDval) in app.atomList.keys()):
#        for coreid in range(0, numCores):
#            atom_file.write("wire: tr_%d_%d:ff_cp_%d_%d_%d:ff_kk%d\n"%(state.IDval, coreid, state.IDval,coreid,coreid, coreid))
    
#    atom_file.write("conf:")    

#    print("printing all fifos now....")
#    for f in fifos:
#        print("fifo:%s\n"%f)
#    return fifos


###########################################################################

def printconf(app, fa):

    for block in app.getBlocks():
        if(block.has_conf == 1):
            fa.write("conf:%s:"%block.name)
            for key in block.cf.keys():
                fa.write("%s,"%block.cf_val[key])

def genAtomsTxt(app, hwMgr):
    numCores = hwMgr.getNumCores()
    numStates = app.getNumStates()
    atomList = app.getAtoms() 
    makeR2Graph(app, hwMgr)
    
################################ FILE END ################################### 
