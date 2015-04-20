'''
Atomix project, orapp.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import abc

INFINITY = 999999

class OrApp:

  __metaclass__ = abc.ABCMeta

  def __init__(self):
    self.states = []
    self.blocksUsed = []
    self.atoms = [];
    self.blockCore = {}        #store core assignments to blocks
    self.atomList = {}
    self.jump_decision = ["BlockDX", "BlockJ"];

    self.out_edges = {}
    self.in_edges = {}

    self.wires = {}
    self.slack = INFINITY
    self.thr = 0


  def setThr(self, thr):
    self.thr = thr
    # TODO: Distribute appropriate throughput reqs
    # to all states
    for state in self.states:
        state.setThr(thr)
 
  def setSlack(self, dead):
    self.slack = dead
    # TODO: Distribute appropriate deadlines to states
    # For now, just send the same deadline to states
    for state in self.states:
        state.setSlack(dead)
    

  def addState(self, state):
    if state not in self.states:
      self.states.append(state)
      state_blocks = state.getBlocks()
      for stateblock in state_blocks:
        if stateblock not in self.blocksUsed:
          self.blocksUsed.append(stateblock)    
    #Note: this assumes that you have called addState after adding
    # all actions to the state
    
  def getStateName(self, state_id):
    for state in self.states:
        if(state.IDval == state_id):
            return state.stateName
    return None
  
  def getStateByID(self, state_id):
    for state in self.states:
      if(state.IDval == state_id):
        return state
    return None

####### TODO FIX #######

  def wire(self, port1, port2, state1_id=-1, state2_id=-1):
    print("Self Wire: ports of blocks %d %d"%(port1.pBlock.IDval, port2.pBlock.IDval))
    self.wires[(port1.pBlock.IDval, state1_id, port1.portname)] = (port2.pBlock.IDval, state2_id, port2.portname)


  def getAllWires(self):
    return self.wires

  def blockIsStateLess(self, bid):
    for block in self.blocksUsed:
        if(block.IDval == bid):
            if(block.has_state == True):
                return False
            else:
                return True
    print("Error: Block with id %d not found\n" %bid)


  def getAtomsForBlock(self, bid):
    atoms_block = []
    for state in self.states:
        if(bid in state.axn.atoms.keys()):
            atoms_block.append(state.axn.atoms[bid])
    
    return atoms_block
    
  def getAtomsForBlockState(self, bid, stateID):
    atoms_block = []
    for state in self.states:
        if(state.IDval == stateID):
            if(bid in state.axn.atoms.keys()):
                atoms_block.append(state.axn.atoms[bid])
    
    return atoms_block

  def getDecisionAtoms(self):
    ret_atoms = []
    for at_key in self.atomList:
        atom = self.atomList[at_key]
        if(atom.atomType == "decision_t"):
            ret_atoms.append(atom)

    return ret_atoms
    

  # We need to know which state is the block in when drawing
  # edges out of a stateful block
#  def wire(self, state1, blocka, state2, blockb):
#    sid1 = state1.IDval
#    sid2 = state2.IDval
#    bid1 = blocka.IDval
#    bid2 = blockb.IDval

#    if((sid1, bid1) in self.out_edges.keys()):
#        self.out_edges[(sid1, bid1)].append((sid2, bid2))
#    else:
#        self.out_edges[(sid1, bid1)] = []
#       self.out_edges[(sid1, bid1)].append((sid2, bid2))

#    if((sid2, bid2) in self.in_edges.keys()):
#        self.in_edges.append((sid1, bid1))
#    else:
#        self.in_edges[(sid2, bid2)] = []
#        self.in_edges[(sid2, bid2)].append((sid1, bid1))


# def wire(self, port1, port2):
#    print("ports of blocks %d %d"%(port1.pBlock.IDval, port2.pBlock.IDval))
#    start_block = self.getBlock(port1.pBlock.IDval)
#    end_block = self.getBlock(port2.pBlock.IDval)
#
    # if one of these blocks is stateful, we will need the state to which this belongs
    # for the stateless block, get the instance of this block in all states and then 
    # draw wires from the stateful block
    # else draw wires from all instances of stateless blockA to state blockB
    # Store it as wires and edges.
    # When the time comes to insert fifos, during R2 generation, it will be easy
#    if(start_block.hasState == False and end_block.hasState == False):
        # find these blocks in all states
#        for state in self.States():
#            if(state.hasBlock(start_block.IDval)):
#                self.wires[(start_block.IDval, port1.portname)] = end_block.IDval
#    self.out_edge(start_block, end_block)

    # store the wire for these ports
#    self.wires[(start_block.IDval, port1.portname)] = end_block.IDval 
    
     

#  def getExitEdges(self, sid, bid):
#    if ((sid, bid) in self.out_edges.keys()):
#        return self.out_edges[(sid, bid)]
        
 
##### TODO FIX END #######
 
  def getBlocks(self):
    return self.blocksUsed   
  
  def getStates(self):
    return self.states

  def getInitState(self):
    for state in self.states:
        if(state.isInitState is True):
            return state

  def getNumStates(self):
    numstates = len(self.states)
    return numstates

  def getAtoms(self):
    return self.atoms    
   
  @abc.abstractmethod
  def declareApp(self):
    pass

class OrIoPort:
  def __init__(self, typename, port_name):
    self.porttype = typename
    self.portname = port_name

class OrIoPortCollection:
    def __init__(self, pBlock):
        self.null = OrInpPort('null', 'null', pBlock)
  

class OrBlock:
  curr_id = 1
    
  def __init__(self, inp_list=[], out_list=[], has_conf=False):
    self.inp_list = inp_list
    self.out_list = out_list
    self.has_conf = has_conf
    self.IDval = OrBlock.curr_id
    self.inp = OrIoPortCollection(self)
    self.out = OrIoPortCollection(self)
    self.portnamearray = []
    OrBlock.curr_id += 1
    print("Created Block ID %d \n" %self.IDval)

  def getid(self):
    return self.IDval 

  def getExitStates(self):
    # This routine will check if this block is a rule block
    # If it is, it's conf is checked for state values to which it can jump
    # NOTE: Somewhat hacky, messy and error-prone for now.. Must fix as code grows                          
    state_ids = []                                                                                          
    if (self.has_conf == 1):                                                                                
      for key in self.cf.keys():                                                                           
        state_id = self.cf_val[key]
        state_ids.append(state_id)
    else:                                                                                                   
      print("Is this an error? Block %s has no conf..\n" %self.blockName)                                   
    return state_ids   

class OrInpPort(OrIoPort):

    def __init__(self, port_type, port_name, pblock):
        OrIoPort.__init__(self, port_type, port_name)
        self.pBlock =  pblock
        #print("initiated port %s for block id %d" %(port_name, pblock.IDval))

class OrOutPort(OrIoPort):
    def __init__(self, port_type, port_name, pblock):
        OrIoPort.__init__(self, port_type, port_name)
        self.pBlock =  pblock
        #print("initiated port %s for block id %d" %(port_name, pblock.IDval))


class OrAtom:
  def __init__(self, blockType, parentDataFlowGraph, core,atomtype,stateid,atomname, pnamearray=[]):
    self.block = blockType
    self.dfg = parentDataFlowGraph # I don't know what to do with this; Kanthi
    self.coreID = core 
    self.stateID = stateid
    self.atomName = atomname
    self.atomType = atomtype
    self.inp = {}
    self.outp = {}
    self.portnames = pnamearray
    self.portFifo = {}


  def getPortnames(self):
    return self.portnames

  def addInp(self, ffname, portname, atomname):
    key = ffname #+"_"+portname
    if(ffname in self.inp.keys()):
        self.inp[key].append(atomname)
    else:
        self.inp[key] = []
        self.inp[key].append(atomname)

    if(portname in self.portFifo.keys()):
        print("ERROR: port %s already has fifo %s \n" %(portname, ffname))
    self.portFifo[portname] = ffname
    
    
  def getPortFifo(self, portname):
    if portname not in self.portFifo.keys():
        print("ERROR: Atom %s Port %s has no fifo with it\n" %(self.atomName, portname))
    else:
        return self.portFifo[portname]

#######################

  def addOutp(self, ffname, portname, end_atom, end_atom_port):
    key = ffname  #+"_"+portname
    if(ffname in self.outp.keys()):
        self.outp[key].append(end_atom)
    else:
        self.outp[key] = []
        self.outp[key].append(end_atom)

    if(portname in self.portFifo.keys()):
        print("ERROR: port %s already has fifo %s \n" %(portname, ffname))
    self.portFifo[portname] = ffname
    # reverse add
    end_atom.addInp(ffname, end_atom_port, self)

  def getDecisionPort(self):
    return "bOutDecision"

class OrAxn:
  #def __init__(self, name):
  def __init__(self):
    #self.name = name
    self.blocks = [] 
    self.wires = {}
    self.edges = []
    self.num_blocks = 0  
    self.localID = {}
    self.globalID = {}
    self.atoms = {}
    self.block_by_name = {}
    self.block_by_id = {}
    self.stateid = 0
  
  #def addAtom(self, atomName, blockType):
  #  if atomName in self.atoms:
  #    raise Exception("Duplicate atom name %s in DataFlowGraph %s" % (atomName, self.name))
  #  atom = Atom(atomName, blockType, self)
  #  self.atoms[atomName] = atom
  #  return atom

  def addAtom(self, blockid, atom):
    self.atoms[blockid] = atom;     

  def add(self, block):
    self.blocks.append(block)
    self.block_by_name[block.name] = block
    self.block_by_id[block.IDval] = block
    self.localID[block.IDval] = self.num_blocks 
    self.globalID[self.num_blocks] = block.IDval
    self.num_blocks = self.num_blocks + 1
    #print("Adding block with ID %d and giving it index %d name %s"%(block.IDval, self.localID[block.IDval], block.name))
    # reverse lookup is important
    
  def getBlockGlobalNameID(self, bid):

    for block in self.blocks:
        if(self.localID[block.IDval] is bid):
            return (block.IDval, block.name)
    print("getBlockGlobalNameID: Invalid block id requested %d\n" %bid);
    return (-1,"NULL");

  def getBlockGlobalID(self, bid):
    if(bid > 0 and bid < len(globalID)):
        return globalID[bid];
    else:
        print("getBlockGlobalID: Error Invalid block id %d requested\n" %bid);
        return -1;
    
  def getBlock(self, bname):
    if bname in self.block_by_name.keys():
        return self.block_by_name[bname]
    else:
        print("getBlock: no block by name %s" %bname)
    return None 

  def getBlockByID(self, bid):
    if bid in self.block_by_id.keys():
        return self.block_by_id[bid]
    else:
        print("getBlockByID: no block by id %d" %bid)
    return None 

  def wire(self, port1, port2):
#    self.wires.append((port1, port2))
    print("ports of blocks %d %d"%(port1.pBlock.IDval, port2.pBlock.IDval))
    print("WIRING: Port names %s %s\n" %(port1.portname, port2.portname))
    start_block = self.getBlockByID(port1.pBlock.IDval)
    end_block = self.getBlockByID(port2.pBlock.IDval)
    # call the edge on these blocks
    self.edge(start_block, end_block)

    # store the wire for these ports
    # simple error check
    if((start_block.IDval, port1.portname) in self.wires.keys()):
      print("Error: Connecting same port twice..\n")
    else:
      self.wires[(start_block.IDval, port1.portname)] = (end_block.IDval, port2.portname) 
    

  def edge(self, start_block, end_block):
    # TODO: Check if this block exists
    #print("Adding edge with ID %d %d " %(start_block.IDval, end_block.IDval))
    #if(start_block.IDval in self.localID.keys() && end_block.IDval in self.localID.keys()):
    id1 = self.localID[start_block.IDval]
    id2 = self.localID[end_block.IDval]
    #print("Index for id %d = %d\n" %(start_block.IDval, id1))
    #print("Index for id %d = %d\n" %(end_block.IDval, id2))

    # NOTE !! Edges are between private indices. Each action has a private
    # index for a block, for ease of scheduling purposes
    # If you want the edges in this action, according to the original blockids,
    # you have to convert these indices to blockids 
    self.edges.append((id1, id2))

  def getEdges(self, b_id):
    # This block id is original ID.. get the equivalent of id in this state
    if(b_id not in self.localID.keys()):
        print("Error !! Invalid bid in getEdges %d\n"%b_id)
    local_id = self.localID[b_id]
    out_edge_ids = []
    in_edge_ids = []
    for edge in self.edges:
        if(edge[0] == local_id):
            out_edge_ids.append(self.globalID[edge[1]])
        if(edge[1] == local_id):
            in_edge_ids.append(self.globalID[edge[0]])
    # Now we have global ids of these edge blocks
    return (in_edge_ids, out_edge_ids)

  def getWires(self, b_id):
      port_wires = {}
      for port_key in self.wires.keys():
          if(port_key[0] == b_id):
              port_wires[port_key[1]] = self.wires[port_key]
              #print("getWires: %d %s\n" %(port_key[0], port_key[1]))
      return port_wires

  
  def getAllWires(self):
    return self.wires
                

  def getFirstBlock(self):
    # Determine the block which has no incoming edges.
    # That is the first block

    isFirst = []
    for bid in range(0, len(self.blocks)):
        isFirst.append(1)
    
    for edge in self.edges:
        s = edge[1];
        isFirst[s] = 0
    
    for bid in range(0, len(self.blocks)):
        if(isFirst[bid] == 1):
            #print("%d doesn't have an incoming edge.. blockid %d\n" %(bid, self.globalID[bid]));
            return self.globalID[bid]
   

  def getBenchMark(self, hwMgr):
    runtime = {}
    # For each of the blocks, read benchmark info from file
    # and populate the p graph
    # Open the benchmark file
    benchmark_file = hwMgr.getBenchMarkFile()
    f = open(benchmark_file, "r")
    for line in f:
       rec = line.split(" ")
       blID = int(rec[0])
       hwID = int(rec[1])
       bm = float(rec[2])
       runtime[(blID, hwID)] = bm 
    f.close()
    # Get number of columns in this matrix.
    # Number of columns is same as number of hw instances
    num_hw = hwMgr.getNumHw()
    # From the runtime dict, extract p array
    blocks = self.getBlocks()
    #print("Setting up array with %d columns %d rows"%(num_hw, len(blocks)))
    p = [[0 for x in xrange(num_hw)] for x in xrange(len(blocks))] 
    for block in blocks:
        for hwid in range(0, num_hw):
            #print("bid %d hwid %d\n" %(block.IDval, hwid))
            if((block.IDval, hwid) in runtime.keys()):
                rtime = runtime[(block.IDval, hwid)]
                p[self.localID[block.IDval]][hwid] = rtime
            else:
                p[self.localID[block.IDval]][hwid] = INFINITY
    
    return p 
     
  def getGraph(self):
    G = []
    # G is like an adjacency list
    # each row contains a dependency
    for edge in self.edges:
      G.append(edge)
      #print("Adding edge %s %s\n"%(edge[0], edge[1]))  
    return G;       
     
  def getBlocks(self):
    return self.blocks

class OrState:
  curr_id = 0

  def __init__(self, app, name):
    self.IDval = OrState.curr_id
    self.isInitState = False
    OrState.curr_id += 1
    #print 'Constructed state with id=%d' % self.IDval
    self.stateName = name
    self.appRef = app
    self.state_slack = INFINITY
    self.state_thr = 0

  def ID(self):
    return self.IDval

  # just to keep it consistent
  def getID(self):
    return self.ID()

  def setAxnAndRule(self, axn, rule):
    self.axn = axn
    self.rule = rule
    self.axn.stateid = self.IDval

  def setAsInitState(self):
    self.isInitState = True

  def getAction(self):
    return self.axn

  def getRule(self):
    return self.rule

  def getBlocks(self):
    return self.axn.getBlocks()

  def getEdges(self, blockid):
    return self.axn.getEdges(blockid)

  def getWires(self, blockid):
    return self.axn.getWires(blockid)

  def getFirstBlock(self):
    return self.axn.getFirstBlock()

  def getAllWires(self):
    return self.axn.getAllWires()

  def setSlack(self, dd):
    self.state_slack = dd

  def getSlack(self):
    return self.state_slack

  def setThr(self, thr):
    self.state_thr = thr

  def getThr(self):
    return self.state_thr

  def getNextNodes(self):
    rBlock = self.rule
    exit_states = []
    # Get the configuration of this rBlock
    exit_state_ids = rBlock.getExitStates()
    for sid in exit_state_ids:
      state_ptr = self.appRef.getStateByID(sid)
      exit_states.append(state_ptr)
    return exit_states

