#!/usr/bin/python
'''
Atomix project, genR2.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from orapp import *

class r2Graph:
    
    def __init__(self, app, hwmgr):
        self.app_ptr = app
        self.num_cores = hwmgr.getNumCores()
        self.states = app.getStates()     
        self.blocks = app.getBlocks()
        self.num_states = len(self.states)
        self.atoms_list = app.atomList
        # list of kernel atoms
        self.kernel_atoms = {}
        self.copy_atoms = {}
        self.transfer_atoms = {}
        self.transfer_atom_num = {}
        self.copy_atom_num = {}
        self.fftype = {}
        self.ffsize = {}
        self.fifo_written = {}
            

    def createKernelAtoms(self):
        ports = ["inp"]
        for core_id in range(1, self.num_cores+1):
            k_atom_name = "kk"+str(core_id)
            k_atom = OrAtom("BlockKK", "NULL", core_id, "kernel", -1, k_atom_name, ports)
            self.kernel_atoms[core_id] = k_atom
             
    def get_kernel_atom(self, core_id):
        return self.kernel_atoms[core_id];

    def setTypeSize(self, ffname, ff_type, ff_size):
        self.fftype[ffname] = ff_type
        self.ffsize[ffname] = ff_size


    def gen_tr_atom(self, stateid, coreid, type_t):
        if((stateid, coreid) in self.transfer_atom_num.keys()):
            # We have already generated a transfer atom for this core to this state
            # generate the next with an appropriate number
            # earlier: return self.transfer_atoms[(stateid, coreid)]
            num_tr = self.transfer_atom_num[(stateid, coreid)]
        else:
            # We have never generated a transfer atom for this combination
            # Let's do it now
            num_tr = 0
        if(stateid == -1):
          statename = "none"
        else:
          statename = str(stateid)
        tr_atom_name = "tr_"+statename+"_"+str(coreid)+"_"+str(num_tr)
        num_tr = num_tr + 1;
        self.transfer_atom_num[(stateid, coreid)] = num_tr
        # Create the transfer atom now
        ports = ["inp", "outp"]
        tr_atom = OrAtom("BlockTR", "NULL", coreid, type_t, stateid, tr_atom_name, ports)
        if((stateid, coreid) in self.transfer_atoms.keys()):
            self.transfer_atoms[(stateid, coreid)].append(tr_atom)
        else:
            self.transfer_atoms[(stateid, coreid)] = []
            self.transfer_atoms[(stateid, coreid)].append(tr_atom)
        return tr_atom

    def gen_cp_atom(self, stateid, coreid, type_t, numCores):
        if((stateid, coreid) in self.copy_atom_num.keys()):
            # We have already generated a transfer atom for this core to this state
            # generate the next with an appropriate number
            # earlier: return self.transfer_atoms[(stateid, coreid)]
            num_cp = self.copy_atom_num[(stateid, coreid)]
        else:
            # We have never generated a transfer atom for this combination
            # Let's do it now
            num_cp = 0
        cp_atom_name = "cp_"+str(stateid)+"_"+str(coreid)+"_"+str(num_cp)
        num_cp = num_cp + 1;
        self.copy_atom_num[(stateid, coreid)] = num_cp
        # Create the transfer atom now
        # Copy atoms has as many outputs as number of cores
        ports = ["inp"]
        for c in range(1, numCores+1):
            out_port = "out_"+str(c)
            ports.append(out_port)    
        cp_atom = OrAtom("BlockCP", "NULL", coreid, type_t, stateid, cp_atom_name, ports)
        if((stateid, coreid) in self.copy_atoms.keys()):
            self.copy_atoms[(stateid, coreid)].append(cp_atom)
        else:
            self.copy_atoms[(stateid, coreid)] = []
            self.copy_atoms[(stateid, coreid)].append(cp_atom)
        return cp_atom

    def get_tr_atom(self, stateid, coreid, type_t):
        if((stateid, coreid) in self.transfer_atoms.keys()):
            tr_atom_list = self.transfer_atoms[(stateid, coreid)]
            for tratom in tr_atom_list:
                if(tratom.atomType is type_t):
                    return tratom
        else:
            return None 


    def get_cp_atom(self, stateid, coreid):
        if((stateid, coreid) in self.copy_atoms.keys()):
            return self.copy_atoms[(stateid, coreid)]

    def get_ffname(self, atom, out_port, atom_type, fifo_size):
       ffname="ff_" + out_port + "_" + atom.atomName
       self.setTypeSize(ffname, atom_type, fifo_size)
       #print("DEBUG: get_ffname : %s %s Returning %s \n" %(atom.atomName, out_port, ffname))
       return ffname 

    
    # This routine detects if there are any stateful atoms that have been used in different states
    # If they have been, we just add fifos to mimic the stateful behavior

    def addStateWires(self):
        app = self.app_ptr
        for state in app.getStates():
            for block in app.getBlocks():
                if((block.IDval, state.IDval) in app.atomList.keys()):
                    atom = app.atomList[(block.IDval, state.IDval)]
                    if(block.has_state == True):
                        # Does this atom have a soul-twin in another state?
                        # By that we mean, whether we have another instance of this stateful
                        # block in any other state?
                            for another_state in app.getStates():
                                if(another_state.IDval == state.IDval):
                                    continue
                                else:
                                    if((block.IDval, another_state.IDval) in app.atomList.keys()):
                                        # There is another atom in another state with the same blockID
                                        # add fifos and wires for state transfer
                                        an_atom = app.atomList[(block.IDval, another_state.IDval)]
                                        state_inp_port = "bInpState"
                                        state_out_port = "bOutState"
                                        ffname = self.get_ffname(atom, state_out_port, atom.atomType, 2)
                                        # Is the soul-mate on a different core? then, add a transfer atom too
                                        if(an_atom.coreID != atom.coreID):
                                            tr_atom = self.gen_tr_atom(state.IDval, an_atom.coreID, atom.atomType)
                                            atom.addOutp(ffname, state_out_port, tr_atom,"inp" ) 
                                            tr_ffname = self.get_ffname(tr_atom, "outp", atom.atomType, 2)
                                            tr_atom.addOutp(tr_ffname, "outp", an_atom, state_inp_port)
                                        else:
                                            atom.addOutp(ffname, state_out_port, an_atom, state_inp_port)


    def getAtoms(self, app, sBlockID, sStateID):    
     #if (app.blockIsStateLess(sBlockID) and (sStateID == -1)):
     if (sStateID == -1):
         # get all atoms for start block if it is a stateless block
         # and if it is between states
         sAtoms = app.getAtomsForBlock(sBlockID)
         ### DEBUG INFO ###
         for sa in sAtoms:
             print("Returned atom %s for block %d" %(sa.atomName, sBlockID))
     else:
         #If the block is stateful, then each edge is different
         #Note this call will return only one atom
         sAtoms = app.getAtomsForBlockState(sBlockID, sStateID)
         for sa in sAtoms:
             print("Returned atom %s for block %d" %(sa.atomName, sBlockID))
     return sAtoms
        
    def addWires(self):

        app = self.app_ptr
        blocks = app.getBlocks()
       
        for block in blocks:
        # Get all the edges out of this block in all states 
            for state in app.getStates():
               state_edges = state.getWires(block.IDval)
               # Insert this into a main list of edges 
               for sedge in state_edges.keys():
                 port_name = sedge
                 other_end = state_edges[sedge]
                 e_bid = other_end[0]
                 e_port = other_end[1]  
                 if((block.IDval, state.IDval, port_name) in  app.wires.keys()):
                    print("Error:Duplicate edge inserted for block %s state %d port %s\n" %(block.IDval, state.IDval, port_name))
                 else:
                    app.wires[(block.IDval, state.IDval, port_name)] = (e_bid, state.IDval, e_port)

        ##print them all now
        for k in app.wires.keys():
            edge_end = app.wires[k]
            print(" Edge (Block %d state %d port %s) = (%d %d %s)\n" %(k[0], k[1], k[2], edge_end[0], edge_end[1], edge_end[2]))

        ## Add fifos between atoms
        for k in app.wires.keys():
            edge_end = app.wires[k]
            sBlockID = k[0]                 # start block
            sStateID = k[1]
            sPortName = k[2]
            eBlockID = edge_end[0]          # end block
            eStateID = edge_end[1]
            ePortName = edge_end[2]
            
            print("EDGE BETWEEN %d - %d in state %d %d portnames %s %s\n" %(sBlockID, eBlockID, sStateID, eStateID, sPortName, edge_end[2]))
            if(sPortName == "null"):
                # There's no data transfer in a null port. So, skip
                continue;

            sAtoms = self.getAtoms(app, sBlockID, sStateID)
            # Now, if the block is stateless and has many instantiations, then all of them
            # should write to the same fifo
            ffname = self.get_ffname(sAtoms[0], sPortName, sAtoms[0].atomType, 10)
#            self.combine_attach_output_fifo(ffname, sAtoms, sPortName)    
            
            # Get the other end atoms 
            eAtoms = self.getAtoms(app, eBlockID, eStateID)
            for satom in sAtoms:
                for eatom in eAtoms:
                    # If this atom is on some other core, we need a transfer block
                    #print("Adding a fifo between %s %s\n" %(satom.atomName, eatom.atomName))
                    if(eatom.coreID != satom.coreID):
                        #get the transfer atom for this state, for this core
                       tr_atom = self.gen_tr_atom(sStateID, eatom.coreID, satom.atomType)
                       #attach_input_fifo(ffname, tr_atom, "inp_null") 
                       satom.addOutp(ffname,sPortName, tr_atom, "inp")
                       tr_ffname = self.get_ffname(tr_atom, "outp", satom.atomType, 10)
                       tr_atom.addOutp(tr_ffname, "outp", eatom, ePortName)
                    else:
                       satom.addOutp(ffname, sPortName, eatom, ePortName)
        # Some blocks don't have any output edges
        dec_atoms = app.getDecisionAtoms()
        for datom in dec_atoms:
            dec_portname = datom.getDecisionPort()
            cp_atom = self.gen_cp_atom(state.IDval, datom.coreID, datom.atomType, self.num_cores)
            ffname = self.get_ffname(datom, dec_portname, datom.atomType, 10)
            datom.addOutp(ffname, dec_portname, cp_atom, "inp")
            #print("DECISION:added atom %s as output for fifo %s" %(cp_atom.atomName, ffname)) 
            for coreid in range(1, (self.num_cores+1)):
                cp_ffname = "ff_cp_"+"%d_%d_%d"%(state.IDval, datom.coreID, coreid)
                self.setTypeSize(cp_ffname, datom.atomType, 10)
                #print("DECISION:added cp_atom %s output fifo %s" %(cp_atom.atomName, cp_ffname)) 
                # These are the transfer atoms transferring decisions to core blocks
                tr_atom = self.get_tr_atom(state.IDval, coreid, "decision_t")
                if(tr_atom is None):
                    tr_atom = self.gen_tr_atom(state.IDval, coreid, "decision_t")
                # Get the right port for this core
                outport = "out_"+str(coreid)
                cp_atom.addOutp(cp_ffname, outport, tr_atom, "inp")
                #print("DECISION: Transfer atom %s copies output for %s %s" %(tr_atom.atomName, cp_atom.atomName, cp_ffname)) 


        # Take care of the rest of the wiring. Wire the output from fifos of transfer blocks
        # to kernel atoms
        for coreid in range(1, (self.num_cores+1)):
            katom = self.get_kernel_atom(coreid)
            kk_ffname = "ff_"+katom.atomName
            self.setTypeSize(kk_ffname, "decision_t", 10)
           # This fifo connects all transfer atoms for all states to this kk atom
            for state in app.getStates():
                stateid = state.IDval
                tratom = self.get_tr_atom(stateid, coreid, "decision_t")
                if(tratom is None):
                    print("ERROR:tratom not found for %d %d\n" %(stateid, coreid))
                else:
                    tratom.addOutp(kk_ffname, "outp", katom, "inp")
#                katom.addInp(kk_ffname, "inp", tratom, "o)

    def printGraph(self):
        app = self.app_ptr
        state = app.getInitState()
        for state in app.getStates():
            blockid = state.getFirstBlock()
            atom = app.atomList[(blockid, state.IDval)]
            # This is the fist atom
        
            self.bfs(atom)

    def bfs(self, atom):
        queue = []
        visited = {}
        queue.append(atom)
        print ("Appending %s first "%atom.atomName); 
        while queue:
            node = queue.pop(0)
            visited[node] = True
            print("\n BEG: %s: " %node.atomName),
            for fifo in node.outp.keys():
                print("%s (%s, %d)"%(fifo, self.fftype[fifo], self.ffsize[fifo])),
                adj_atoms = node.outp[fifo]
                for adj_atom in adj_atoms:
                    print("%s "%adj_atom.atomName),
                    if(adj_atom not in visited.keys() or visited[adj_atom] is not True):
                        queue.append(adj_atom)

    def printFifo(self, ffname, core_id, fifo_file):
        if(ffname not in self.fifo_written.keys()):
            self.fifo_written[ffname]  = 1
            fifo_file.write("fifo: %s:%d:sin:%d\n" %(ffname, self.ffsize[ffname], core_id))

    def printWire(self, atom_file, fifo_file, at):
        atname = at.atomName
        atom_file.write("wire:%s"%atname),
        #for ffnames in at.inp.keys():
        #    atom_file.write(":%s" %ffnames),
            #self.printFifo(ffnames, at.coreID, fifo_file)
        for ports in at.getPortnames():
            ffnames = at.getPortFifo(ports)
            #print("atom %s port %s ffname %s" %(atname,ports, ffnames))
            if ffnames is not None:
                atom_file.write(":%s" %ffnames),
                self.printFifo(ffnames, at.coreID, fifo_file)
            else:
                atom_file.write(":NO_FIFO"),
        atom_file.write("\n")
    
    
    def printWires(self, atom_file, fifo_file):
        for at in self.app_ptr.getAtoms():
            self.printWire(atom_file, fifo_file, at)

        atom_file.write("\n# transfer atoms\n")
        fifo_file.write("\n# transfer fifos\n")
        for tr_key in self.transfer_atoms.keys():
            at_list = self.transfer_atoms[tr_key]
            for at in at_list:
                self.printWire(atom_file, fifo_file, at)

        atom_file.write("\n# copy atoms\n")
        fifo_file.write("\n# copy fifos\n")
        for cp_key in self.copy_atoms.keys():
            at_list = self.copy_atoms[cp_key]
            for at in at_list:
                self.printWire(atom_file, fifo_file, at)

        atom_file.write("\n# kernel atoms\n")
        fifo_file.write("\n# kernel fifos\n")
        for kk_key in self.kernel_atoms.keys():
            at = self.kernel_atoms[kk_key]
            self.printWire(atom_file, fifo_file, at)

    def printAtom(self, atom_file, at):
            c = at.coreID;
            state = at.stateID;
            name = at.block;
            atname = at.atomName;
            atom_file.write("atom: %s: %s: %d\n" %(atname, name, c))
        

    def printAtoms(self, atom_file):
        for at in self.app_ptr.getAtoms():
            self.printAtom(atom_file, at)

        for tr_key in self.transfer_atoms.keys():
            at_list = self.transfer_atoms[tr_key]
            for at in at_list:
                self.printAtom(atom_file, at)

        for cp_key in self.copy_atoms.keys():
            at_list = self.copy_atoms[cp_key]
            for at in at_list:
                self.printAtom(atom_file, at)
        
        for kk_key in self.kernel_atoms.keys():
            at = self.kernel_atoms[kk_key]
            self.printAtom(atom_file, at)
            
        atom_file.write("\n\n")
    
    def printAtomConf(self, atom_file):
        for state in self.app_ptr.getStates():
            for block in self.app_ptr.getBlocks():
                if((block.IDval, state.IDval) in self.app_ptr.atomList.keys()):
                    # Ok, there is an atom.
                    if(block.has_conf == 1):
                        atom_file.write("conf:%s:"%self.app_ptr.atomList[(block.IDval, state.IDval)].atomName)
                        for key in block.cf.keys():
                            state_id = block.cf_val[key]
                            state_name = self.app_ptr.getStateName(state_id)
                            atom_file.write("%s,"%state_name)
                        atom_file.seek(-1, 2)
                        atom_file.write("\n")
                    
        # NOTE: TBD: Print config information for regular atoms
        conf_msg = "ROUND_UP_N_BYTES_TO_DOUBLE_WORD_MULTIPLE"
 
        # print the config for TR atoms
        for tr_key in sorted(self.transfer_atoms.keys()):
            at_list = self.transfer_atoms[tr_key]
            for at in at_list:
                atom_file.write("conf:%s:%s(sizeof(%s))\n" %(at.atomName, conf_msg, at.atomType)) 

    def printFifoConf(self, fifo_file):
        for ff_key in sorted(self.fftype.keys()):
            if(self.fftype[ff_key] == "decision_t"):
                fifo_file.write("typehint:%s:%s\n"%(ff_key,self.fftype[ff_key]))     

