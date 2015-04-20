#!/usr/bin/python
'''
Atomix project, orpy_util.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

from _scrape_b_i import *
from hw_info import *
import numpy as np
import os
import gencplex as gcplex
import subprocess
from orapp import *
from genR2 import *

def import_blocks(logger, using_blocks, ilib_include_path):
  
  #out_dir = '__orblocks__'
  #if not os.path.exists(out_dir):
  #  os.makedirs(out_dir)

  fname = '__orblocks__.py'
  print 'Importing blocks into %s/%s' % (os.getcwd(), fname)
  print("%s\n" %ilib_include_path)
  f = open(fname, 'w')

  blockScraper = BlockScraper(logger)

  for blockname in using_blocks:
    print 'Importing block %s' % blockname

    block_inp_file = blockScraper.which(blockname, ilib_include_path)
    ios_list = blockScraper.parse_block_i_h_file(blockname, block_inp_file)
    #print 'atomname=%s, blockname=%s, ios_list:' % (atomname, blockname)
    #print ios_list
    inp_type_list = ios_list[0]
    out_type_list = ios_list[1]
    conf_type_list = ios_list[2]
    inp_dict = ios_list[3]
    out_dict = ios_list[4]
    cf_par_list = ios_list[5]
    cf_par_dict = ios_list[6]

    #inptstr = ','.join(inp_type_list)
    #outtstr = ','.join(out_type_list)
    #conftstr = ','.join(conf_type_list)
    #print 'inptstr:"%s"' % inptstr
    #print 'outtstr:"%s"' % outtstr
    #print 'conftstr:"%s"' % conftstr
    #raw_input()
    if len(conf_type_list) == 0:
      hasConf = 0
    elif len(conf_type_list) == 1:
      hasConf = 1
    else:
      print "ERROR: Block %s has more than one conf types" % blockname
      exit(13)
    #TODO

    
    #write out the block
    #f = open('%s/%s.py' % (out_dir, blockname), 'w')
    port_counter = 0
    f.write('from orapp import *\n')
    f.write('class %s(OrBlock):\n' % blockname)
    f.write('  def __init__(self')
    if hasConf == 1:
      for par in cf_par_list:
        f.write(', %s' % par)
    f.write('):\n')
    f.write('    OrBlock.__init__(self)\n') 
    f.write('    self.name = \"%s\"\n' %blockname)
    for typename, portname in inp_dict:
      #typename = inp_dict[portname]
      f.write('    self.inp.%s = OrInpPort(\'%s\', \'%s\', self)\n' % (portname, typename, portname))
      f.write('    self.portnamearray.append(\'%s\')\n' %(portname)) 
      port_counter+=1
    for typename,portname in out_dict:
      #print '---------------', portname
      #typename = out_dict[portname]
      f.write('    self.out.%s = OrOutPort(\'%s\', \'%s\',self)\n' % (portname, typename, portname))
      f.write('    self.portnamearray.append(\'%s\')\n' %(portname)) 
      if (blockname == "BlockA"):
        f.write('    self.out.bOutData1 = OrOutPort(\'EXILIB_tData_1\', \'bOutData1\',self)\n')
        f.write('    self.portnamearray.append(\'bOutData1\')\n')
        port_counter+=1
      port_counter+=1
    f.write('    self.has_conf = %s\n' % hasConf)
    ## HACK ! Correct this !
    if (blockname == "BlockAS"):
        f.write('    self.has_state = 1\n')
    else:
        f.write('    self.has_state = 0\n')
    f.write('    self.cf = %s\n' % cf_par_dict)
    if hasConf == 1:
        f.write('    self.cf_val = {}\n')
        for k in cf_par_dict.keys():
            f.write('    self.cf_val[\'%s\'] = %s\n' %(k, k))
    f.write('\n')

  f.close()
  print 'Imported blocks into %s/%s' % (os.getcwd(), fname)

def write_benchmark_file(orhwmgr, app):
    orhwmgr.init_benchmark_file()
    blocks = app.getBlocks()  
    for block in blocks:
        #print 'Benchmarking block %d' % block.IDval
        orhwmgr.benchmark_block(block)

def write_cplex_input(state, g, p, q):
    # Generate name of the file
    stateid = state.IDval;
    slack = state.getSlack()
    thr = state.getThr()
    
    # Get the right G string
    filename = "__cgx_state_s"+str(stateid)+"_cg__.py"
    f = open(filename, "w")
    f.write("import numpy as np\n")
    f.write("def model(D):\n")
    pmatrix = "p = "+str(p);
    qmatrix = "q = "+str(q.tolist())
    f.write("   %s\n"%pmatrix)
    # G is more complicated
    f.write("   G = '''\n")
    for row in g:
        f.write("%d %d %d\n"%(row[0], row[1], 1))
    f.write("   ''';\n");
    f.write("   %s\n"%qmatrix)
    f.write("   p1 = np.array(p)\n");
    f.write("   q1 = np.array(q)\n");
    f.write("   slack = %d\n" %slack);
    f.write("   thr = %d\n" %thr);
    f.write("   return (p1, G, q1, slack, thr)\n")
    f.close()
    
def run_cplex(state):
    stateid = state.IDval;
    inp_fname = "__cgx_state_s"+str(stateid)+"_cg__.py"
    #print("using %s\n"%inp_fname);
    gcplex.main(inp_fname)
    inp_fname = "__cgx_state_s"+str(stateid)+"_cg__.cplex"
    #print("cplex filename %s\n"%inp_fname);
    cmd = "cplex < "+inp_fname
    process = subprocess.Popen(cmd, shell=True)
    process.wait()
    
def parse_cplexsol(state):
   stateid = state.IDval;
   inp_fname = "__cgx_state_s"+str(stateid)+"_cg__.sol" 
   f = open(inp_fname, 'r')
   out_strs = []
   for line in f:
     line.strip()
     if "variable " in line:
        elems = line.split('\"')
        if("C_max" not in elems[1]):
            out_str = elems[1]+" "+elems[5]
            out_strs.append(out_str)
   out_strs.sort()
   outp_fname = "__cgx_state_s"+str(stateid)+"_cg__.txsol"
   outp = open(outp_fname, "w")
   for entry in out_strs:
       outp.write("%s\n"%(entry))
   outp.close()
   f.close()

def createAtoms(hwMgr, app):
    states = app.getStates()
    #atoms_list = []
    for state in states:
        stateid = state.IDval;
        #print("state id: %d\n" %stateid)
        inp_fname = "__cgx_state_s"+str(stateid)+"_cg__.txsol";
        #print("checking states in %s" %inp_fname)
        f = open(inp_fname, 'r')
        for line in f:
            line.rstrip()
            # ys are core assignments
            chars = list(line);
            if(chars[0] is 'y'):
                # Check if this core assignment is 1
                coreyes = int(line.split(" ")[1])    
                if(coreyes == 1):
                    block_id = int(chars[2]) - 1
                    core_id = int(chars[4])
                    # Get the original block number and block name
                    (blockid, blockname)  = state.axn.getBlockGlobalNameID(block_id)
                    if(blockname in app.jump_decision):
                        atomtype = "decision_t"
                    else:
                        atomtype = "compute"

                    lastchar = blockname[-1];
                    if(blockname is "BlockDX" or blockname is "BlockAS"):
                        lastchar = blockname[-2:] 

                    atomName = lastchar.lower()+"_"+str(stateid)+"_"+str(core_id);
                    print("Added new atom with %s %d %d %d" %(blockname, core_id, stateid, blockid))
                    axn = state.getAction()
                    block_ptr = axn.getBlockByID(blockid)
                    portnames = block_ptr.portnamearray

                    newAtom = OrAtom(blockname, "NULL", core_id, atomtype, int(stateid), atomName, portnames)
                    app.atoms.append(newAtom)
                    app.blockCore[(blockid, stateid)] = core_id;
                    app.atomList[(blockid, stateid)] = newAtom;
                    
                    axn.addAtom(blockid, newAtom)


def makeR2Graph(app, hwMgr):

    # app has an atomList. We will copy it here
    # we will create kernel atoms and add appropriate fifos to them
    
    R2Graph = r2Graph(app, hwMgr)
    R2Graph.createKernelAtoms()
    #R2Graph.createTransferAtoms()
    #R2Graph.createCopyAtoms()

    R2Graph.addStateWires()

    R2Graph.addWires()

    R2Graph.printGraph()

    f1 = open("atoms.txt", "w")
    f2 = open("fifos.txt", "w")
    R2Graph.printAtoms(f1)
    R2Graph.printWires(f1, f2)
    R2Graph.printAtomConf(f1)
    R2Graph.printFifoConf(f2)
    f1.close()
    f2.close()

def addToGraph(ret_graph, cand_node, next_node):
  if(cand_node in ret_graph.keys()):
    ret_graph[cand_node].append(next_node)
  else:
    ret_graph[cand_node] = []
    ret_graph[cand_node].append(next_node)

#    print("Added %d node to %d key\n" %(cand_node, next_node.getID()))

    return ret_graph

def returnBFSGraph(init_node):
  ret_graph = {}
  # Mark all states as not visited 
  visited = {}
  # Now start with an empty queue
  bfs_queue = []
  bfs_queue.append(init_node)
  while(bfs_queue):
    cand_node = bfs_queue.pop(0)
    visited[cand_node] = True
    next_nodes = cand_node.getNextNodes()
    for nextnode in next_nodes:
      print("next node id is %d \n"%nextnode.getID())
      if(nextnode not in visited.keys() or visited[nextnode] is not True):
        # This state is not yet visited.. add it to the queue
        bfs_queue.append(nextnode)
        # Also add it to the adjacency list, which is the original purpose
        addToGraph(ret_graph, cand_node, nextnode)
  return ret_graph
   

def genGraphforSched(app):
  state_graph = {}
  init_state = app.getInitState()
  state_graph = returnBFSGraph(init_state)

  for keys in state_graph.keys():
    print("SCHEDGRAPH Node: %d" %keys.getID())
    ends = state_graph[keys]
    for end_states in ends:
      print("SCHEDGRAPH: %d " %end_states.getID())

  ## Now in each state graph, get the node-level graphs
  ## We need global identifiers for all the nodes since there are
  ## some edges to nodes beyond each state
  app_wires = app.getAllWires()

  state_block_graph = {}
  for state in app.getStates():
    state_wires = state.getAllWires()
    for wire_key in state_wires.keys():
      start_block_id = wire_key[0]
      end_block_id = (state_wires[wire_key])[0]
      if state not in state_block_graph.keys():
        state_block_graph[state] = {}
      addToGraph(state_block_graph[state], start_block_id, end_block_id)
      for app_wire_key in app_wires.keys():
        if (start_block_id == app_wire_key[0]):
          print("SCHEDGRAPH: BGRAPH: APPGRAPH: YES BLOCK ID %d ADDING TO STATE ID %d" %(start_block_id, state.ID()))
          end_block_id = (app_wires[app_wire_key])[0]
          addToGraph(state_block_graph[state], start_block_id, end_block_id)

  # The state level graphs
  for state in app.getStates():
    bgraph = state_block_graph[state]
    for keys in bgraph.keys():
      print("SCHEDGRAPH BGRAPH STATE %d NODE %d: " %(state.ID(), keys)),
      print(bgraph[keys])

      
  

def create_logger(LOGFILE):
  # create logger with 'pxc'
  logger = logging.getLogger('pxc')
  #logger.setLevel(logging.INFO)
  logger.setLevel(logging.DEBUG)

  # create file handler which logs even debug messages
  fh = logging.FileHandler(LOGFILE, mode='w')
  #fh = logging.FileHandler(LOGFILE, mode='a')
  #fh.setLevel(logging.INFO)
  fh.setLevel(logging.DEBUG)

  # create console handler with a higher log level
  ch = logging.StreamHandler()
  ch.setLevel(logging.INFO)

  # create formatter and add it to the handlers
  formatter = logging.Formatter('%(asctime)s:%(name)s:%(levelname)s:%(message)s')
  fh.setFormatter(formatter)
  ch.setFormatter(formatter)

  # add the handlers to the logger
  logger.addHandler(fh)
  logger.addHandler(ch)

  return logger
