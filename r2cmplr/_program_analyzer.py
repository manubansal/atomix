'''
Atomix project, _program_analyzer.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from _db import *
from _codegen_write_out import *
from _util import *

from collections import Counter

import networkx as nx
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

DELTA = 1e-9

class ProgramConstructor:

  #-----------------------------
  # construct the data flowgraph
  # (atoms, fifos and wires)
  #-----------------------------
  #add all atoms in the program as nodes of the dfg
  def add_atoms(self, ATOM_DB):
    self.logger.info("adding atoms")
    P = self.P
    for row in ATOM_DB:
      print row
      atom, block, core, inp_ports, out_ports = tuple(row)
      inp_ports_list = []
      out_ports_list = []
      if inp_ports:
	inp_ports_list = inp_ports.split(',')
      if out_ports:
	out_ports_list = out_ports.split(',')
      print atom, block, inp_ports_list, out_ports_list
      P.add_node(atom, block=block, inp_ports=inp_ports_list, out_ports=out_ports_list)
    atoms = [row[0] for row in ATOM_DB]
    self.atoms = atoms
    self.ATOM_DB = ATOM_DB


  #add all fifos in the program as nodes of the dfg
  def add_fifos(self, FIFO_DB):
    self.logger.info("adding fifos")
    P = self.P
    for row in FIFO_DB:
      #print row
      fifo, nbufs, fifotype, core = tuple(row)
      print fifo, nbufs, core
      P.add_node(fifo, nbufs=int(nbufs))
    fifos = [row[0] for row in FIFO_DB]
    self.fifos = fifos
    self.FIFO_DB = FIFO_DB

  #add all wires as edges going between atoms and fifos in the dfg
  def add_wires(self, WIRE_DB):
    self.logger.info("adding wires")
    P = self.P
    self.edges = []
    for row in WIRE_DB:
      print row
      (wired_atom, wired_fifos),(_, orig_wired_fifos) = tuple(row)
      print "wired_fifos =|%s|" % wired_fifos
      wired_fifos = wired_fifos.split(',')
      orig_wired_fifos = orig_wired_fifos.split(',')
      print "wired_fifos =|%s|" % wired_fifos
      atom = P.node[wired_atom]
      inp_ports = atom["inp_ports"]
      out_ports = atom["out_ports"]
      ninp = len(inp_ports)
      nout = len(out_ports)
      print inp_ports, out_ports, ninp, nout
      wired_inps = wired_fifos[0:ninp]
      wired_outs = wired_fifos[ninp:]
      orig_wired_inps = orig_wired_fifos[0:ninp]
      orig_wired_outs = orig_wired_fifos[ninp:]
      #print wired_atom, wired_fifos, atom
      #print wired_atom, wired_fifos, inp_ports, out_ports, "wired_inps:", wired_inps, "wired_outs:", wired_outs
      print wired_atom, "wired_inps:", wired_inps, "wired_outs:", wired_outs
      print wired_atom, "orig_wired_inps:", orig_wired_inps, "orig_wired_outs:", orig_wired_outs
      #if wired_atom == 'jumpToRxDataFinish1':
      #  raw_input()
      #wired_inp_fifos = [P.node[fifo] for fifo in wired_inps]
      #wired_out_fifos = [P.node[fifo] for fifo in wired_outs]
      #inp_edges = [(fifo,atom) for fifo in wired_inp_fifos]
      #out_edges = [(atom,fifo) for fifo in wired_out_fifos]
#      if wired_fifos != orig_wired_fifos:
#	raw_input()
      inp_edges = []
      inp_edges_weighted = []
      if wired_inps != orig_wired_inps:
        for fifo, orig_fifo in zip(wired_inps, orig_wired_inps):
	  #if orig_fifo.endswith(">"):
	    #print fifo, orig_fifo
	    #raw_input()
	  if fifo != orig_fifo:
	    op = orig_fifo[-1]
	    print "op = ", op
	    #raw_input()
	    if op == "+" or op == "*":
	      inp_edges.append((fifo,wired_atom,1))
	      inp_edges.append((wired_atom,fifo,1))
	    elif op == ">":
	      inp_edges_weighted.append((fifo,wired_atom,0.5))
	      #print inp_edges_weighted
	      #raw_input()
	    elif op == "<":
	      inp_edges_weighted.append((fifo,wired_atom,0.5))
	    elif op == ")":
	      inp_edges_weighted.append((fifo,wired_atom,0.4))
	    elif op == "(":
	      inp_edges_weighted.append((fifo,wired_atom,0.6))
	    elif op == "[":
	      inp_edges_weighted.append((fifo,wired_atom,0.6))
	      inp_edges_weighted.append((wired_atom,fifo,1))
	    else:
	      print "unhandled case"
	      sys.exit(1)
	  else:
	    inp_edges.append((fifo,wired_atom,1))
      else:
	inp_edges = [(fifo,wired_atom,1) for fifo in wired_inps]

      out_edges = []
      out_edges_weighted = []
      if wired_outs != orig_wired_outs:
        for fifo, orig_fifo in zip(wired_outs, orig_wired_outs):
	  if fifo != orig_fifo:
	    op = orig_fifo[-1]
	    print "op = ", op
	    if op == "+" or op == "*":
	      out_edges.append((fifo,wired_atom,1))
	      out_edges.append((wired_atom,fifo,1))
	    elif op == ">":
	      out_edges_weighted.append((wired_atom,fifo,0.5))
	    elif op == "<":
	      out_edges_weighted.append((wired_atom,fifo,0.5))
	    elif op == ")":
	      out_edges_weighted.append((wired_atom,fifo,0.4))
	    elif op == "(":
	      out_edges_weighted.append((wired_atom,fifo,0.6))
	    else:
	      print "unhandled case"
	      sys.exit(1)
	  else:
	    out_edges.append((wired_atom,fifo,1))
      else:
	out_edges = [(wired_atom,fifo,1) for fifo in wired_outs]

      edges = inp_edges + out_edges
      weighted_edges = inp_edges_weighted + out_edges_weighted

      edges += weighted_edges
      print edges
      ws = [w for u,v,w in edges]
      #if any([w != 1 for w in ws]):
      #  raw_input()

      #P.add_edges_from(edges)
      #P.add_weighted_edges_from(edges)
      #P.add_weighted_edges_from(weighted_edges)

      P.add_weighted_edges_from(edges)

      #self.edges += edges
      #self.weighted_edges += weighted_edges
    self.WIRE_DB = WIRE_DB
  
  def add_conf(self, CONF_DB):
    for row in CONF_DB:
      print row
    self.CONF_DB = CONF_DB

  #---------------------------------
  # construct the control flow graph
  # (actions and states)
  #---------------------------------
  #for each state, collect actions (flowgraphs) and statep's belonging to same
  #state --> these are the nodes of the flowgraph graphs
  def add_actions(self, AXN_DB):
    self.logger.info("adding actions")
    #P = self.P
    #for row in AXN_DB:
    #  print row
    self.AXN_DB = AXN_DB
    adb = AXN_DB
    adbd = {}
    adbd['noaxn'] = []
    for row in adb:
      axn, atoms = tuple(row)
      adbd[axn] = atoms
    self.AXN_DB_DICT = adbd


  #list all states
  def add_states(self, STATE_DB, initStateName):
    self.logger.info("adding states")
    for state in STATE_DB:
      print state, STATE_DB[state]
    self.STATE_DB = STATE_DB
    self.initStateName = initStateName


  #---------------------------------
  # analysis methods
  #---------------------------------
  def analyze(self):
    self.find_dx_atoms()
    #self.find_action_subgraphs()
    #self.find_state_subgraphs()

    paths = []
### TODO: move these out into a configuration file (maybe debug.txt)
###    paths.append(['staRxPktInit', 'staRxLTFProc', 'staRxPktDetect', 'staRxPLCPDecodeSpec', 'staRxContinue', 'staRxFinish'])
###    paths.append(['staRxPktInit', 'staRxLTFProc', 'staRxPktDetect', 'staRxPLCPDecodeSpec', 'staRxContinue', 'staRxFinish'])
####    paths.append(['staRxPktInit', 'staRxLTFProc', 'staRxPktDetect', 'staRxPLCPDecodeSpec', 'staRxDataDecode54m_setup', \
####    	'staRxDataDecode54m_c', 'staRxDataDecode54m_H', 'staRxDataDecode54m_Mp', 'staRxDataDecode54m_M', 'staRxDataDecode54m_M', \
####	'staRxDataDecode54m_T', 'staRxDataDecode54m_f', 'staRxDataFinish', 'staTxAckInit', 'staTxAckPreambleSignal', \
####	'staTxAck', 'staTxAckFinal', 'staRxFinish'])
###    paths.append(['staRxPktInit', 'staRxLTFProc', 'staRxPktDetect', 'staRxPLCPDecodeSpec', 'staRxDataDecode54m_setup', \
###    	'staRxDataDecode54m_H', 'staRxDataDecode54m_Mp', 'staRxDataDecode54m_M', 'staRxDataDecode54m_M', \
###	'staRxDataDecode54m_T', 'staRxDataDecode54m_f', 'staRxDataFinish', 'staTxAckInit', 'staTxAckPreambleSignal', \
###	'staTxAck', 'staTxAckFinal', 'staRxFinish'])

    statepathnames = []
    state_paths_index_file = "__data_conservation_state_paths_index.txt"
    f = open(state_paths_index_file, "w")
    for i, path in enumerate(paths):
      #state_path_name = ""
      f.write("%d %s\n" % (i, path))
      for state in path:
	#state_path_name += "+%s" % state
	#statepathnames.append(state_path_name)
	state_path_name = "path_%d" % i
      subgraph = self.find_statepath_subgraph(path, state_path_name, True)
      subgraph_name = state_path_name
      mismatched_fifos = self.check_data_conservation(subgraph, subgraph_name)
      self.draw_mismatched_fifos(subgraph, subgraph_name, mismatched_fifos)
    f.close()


  def draw_mismatched_fifos(self, subgraph, subgraph_name, mismatched_fifos):
    for f in mismatched_fifos:
      g = nx.MultiDiGraph()
      in_edges = subgraph.in_edges(f,True,True)
      out_edges = subgraph.out_edges(f,True,True)
      edges = in_edges + out_edges
      print "edges:", edges
      g.add_edges_from(edges)
      print "nodes:", g.nodes()
      print "edges:", g.edges()
      g_name = "__data_conservation_" + subgraph_name + "_mismatch_" + f + ".png"
      self.draw_twocolor_(g, True, g_name, False, (10,10))

  def check_data_conservation(self, G, subgraph_name):
    n = G.nodes()
    atoms = set(self.atoms).intersection(n)
    fifos = set(self.fifos).intersection(n)
    din_f = G.in_degree(fifos,weight='weight')
    dout_f = G.out_degree(fifos,weight='weight')
    dinout = {}
    #print din_f
    #print dout_f
    #print dinout
    outfile = "__data_conservation_%s.txt" % subgraph_name
    of = open(outfile, "w")
    mismatched_fifos = []
    for f in fifos:
      dinout[f] = (din_f[f], dout_f[f], din_f[f] - dout_f[f])
      ind,outd,diff = dinout[f]
      #print "%40s" % f, dinout[f]
      cur = ""
      if abs(diff) > DELTA:
	cur = "<<"
	mismatched_fifos.append(f)
      print "%40s %5.1f, %5.1f, %5.1f  %s" % (f, ind,outd,diff,cur)
      of.write("%40s %5.1f, %5.1f, %5.1f  %s\n" % (f, ind,outd,diff,cur))
    of.close()

    #raw_input()
    return mismatched_fifos


  def find_atom_subgraph(self, atoms_set, count_multiplicity=False):
    if not count_multiplicity:
      return self.find_atom_subgraph_no_multiplicity(atoms_set)
    else:
      return self.find_atom_subgraph_with_multiplicity(atoms_set)

  def find_atom_subgraph_with_multiplicity(self, atoms_set):
    G = self.P
    #atoms = self.atoms
    #fifos = self.fifos
    print "atoms_set (with multiplicity):", atoms_set
    print "----------------------------------------"
    subgraph = nx.MultiDiGraph()
    for atom in atoms_set:
      #preds = G.predecessors(atom)
      #succs = G.successors(atom)
      #edges = [(p,atom) for p in preds]
      #print "edges:", edges
      #edges += [(atom,s) for s in succs]
      #print "edges:", edges
      #subgraph.add_edges_from(edges)
      in_edges = G.in_edges(atom, True, True)
      out_edges = G.out_edges(atom, True, True)
      print "in_edges:", in_edges
      print "out_edges:", out_edges
      edges = in_edges + out_edges
#      weights = [d['weight'] for _,_,_,d in edges]
#      if any([w != 1 for w in weights]):
#	raw_input()
      #if atom == "agcStateDrainer":
      #  raw_input()

      #subgraph.add_weighted_edges_from(edges)
      #subgraph.add_edges_from(edges)
      
      weighted_edges = [(u,v,d['weight']) for u,v,_,d in edges]
      subgraph.add_weighted_edges_from(weighted_edges)
      #note: by adding (u,v,w)'s instead of edges directly gets rid of
      #edge identifier that nx produces. this allows us to add the same
      #edge from the full graph multiple times in the subgraph. if, instead,
      #we let the edge id stay, adding the same edge multiple times will 
      #not replicate it as desired.
    
    print subgraph.nodes()
    print subgraph.edges()
    print '---------- printed subgraph nodes and edges -----------'
    return subgraph

  def find_atom_subgraph_no_multiplicity(self, atoms_set):
    G = self.P
    #atoms = self.atoms
    #fifos = self.fifos
    print "atoms_set (with multiplicity):", atoms_set
    print "----------------------------------------"
    preds = []
    succs = []
    for atom in atoms_set:
      preds += G.predecessors(atom)
      succs += G.successors(atom)
    expanded_atoms_set = atoms_set + preds + succs
    print "expanded_atoms_set:", expanded_atoms_set
    print "----------------------------------------"
    c = Counter(expanded_atoms_set)
    print "counts:", c
    print "----------------------------------------"
    raw_input()
    
    subgraph = G.subgraph(expanded_atoms_set)
    return subgraph
    
  def find_statepath_subgraph(self, path=[], state_path_name="", count_multiplicity=False):
    if not path:
      return
    
    #sdb = self.STATE_DB
    state_path_atoms = []
    for state in path:
      atoms = self.find_state_atoms(state)
      state_path_atoms += atoms

    print "state_path_atoms:", state_path_atoms
    print "--------- printed state path atoms-----------"
    atoms = state_path_atoms
    subgraph = self.find_atom_subgraph(atoms, count_multiplicity)

    ## self.draw_simple_(subgraph)

    plotfile = "__%s.png" % state_path_name
    show_labels = True
    show = False
    self.draw_twocolor_(subgraph, show_labels, plotfile, show)
    #if state.startswith('...'):
      #self.draw_twocolor_(subgraph, show_labels, plotfile, show)

    return subgraph

  def find_state_atoms(self, state):
    adbd = self.AXN_DB_DICT
    sdb = self.STATE_DB
    v = sdb[state]
    sfgs_atoms = []
    print state, v
    #state_flowgraphs
    sfgs_atomseqs = v.values()
    sfgs = [sfg for sfg,_ in sfgs_atomseqs]
    atomseqs = [atomseq for _,atomseq in sfgs_atomseqs]
    print sfgs
    print atomseqs
    for fg in sfgs:
      sfgs_atoms += adbd[fg]
    state_atoms = []
    for aseq in atomseqs:
      state_atoms += aseq
    print sfgs_atoms
    print state_atoms
    all_state_atoms = sfgs_atoms + state_atoms
    print all_state_atoms
    print "----printed all state atoms--------"

    atoms = all_state_atoms
    return atoms


  def find_state_subgraphs(self):
    sdb = self.STATE_DB

    for state in sdb:
      atoms = self.find_state_atoms(state)
      subgraph = self.find_atom_subgraph(atoms)

      ## self.draw_simple_(subgraph)

      plotfile = "__%s.png" % state
      show_labels = True
      show = False
      self.draw_twocolor_(subgraph, show_labels, plotfile, show)
      #if state.startswith('...'):
	#self.draw_twocolor_(subgraph, show_labels, plotfile, show)


  def find_action_subgraphs(self):
    adb = self.AXN_DB
    for row in adb:
      print row
      axn, atoms = tuple(row)
      subgraph = self.find_atom_subgraph(atoms)
      #print subgraph

      ## self.draw_simple_(subgraph)

      plotfile = "__%s.png" % axn
      show_labels = True
      show = False
      self.draw_twocolor_(subgraph, show_labels, plotfile, show)
      #if axn.startswith('axnRxPLCPDecodeSpec'):
	#self.draw_twocolor_(subgraph, show_labels, plotfile, show)


  def find_dx_atoms(self):
    ## algorithm 1 ##
    #this is problematic because some decision atoms feed into ff_kk* through
    #transfer atoms, so they are not neighbors of ff_kk* nodes
    #fifos = self.fifos
    #kernel_fifos = [fifo for fifo in fifos if fifo.startswith("ff_kk")]
    #print "kernel_fifos =", kernel_fifos
    #dx_atoms = []
    #for ff_kk in kernel_fifos:
    #  dx_atoms += G.predecessors(ff_kk)
    #print dx_atoms

    ## algorithm 2 ##
    #this one could become problematic if a decision atom output void type 
    #instead of full type Decision_t. for now, it suffices. it would be good
    #to impose enforcement of Decision_t type for a decision atom to quality
    #as a legit decision atom right in the language.
    G = self.P
    atoms = self.atoms
    for atom in atoms:
      print atom, G.node[atom]
    #raw_input()
    dx_atoms = [atom for atom in atoms if any([p.split(':')[0] == 'Decision_t' for p in G.node[atom]["out_ports"]])]
    print dx_atoms
    #raw_input()


    cdb = self.CONF_DB
    cdbd = {}
    for conf in cdb:
      k, v = tuple(conf)
      cdbd[k] = v

    for dxa in dx_atoms:
      print dxa, cdbd[dxa]

    self.dx_atoms = dx_atoms

    #pp = G.predecessors('jumpToRxDataFinish1')
    #ss = G.successors('jumpToRxDataFinish1')
    #print pp, ss
  
  #---------------------------------
  # rendering methods
  #---------------------------------
  def draw(self):
    #self.draw_simple()
    self.draw_twocolor()

  def draw_simple_(self, G):
    nx.draw(G)
    plt.show()
    
  def draw_simple(self):
    nx.draw(self.P)
    plt.savefig("__program.png")
    #plt.show()

      
  def flattened_edge_weights(self, G):
      edges_weights = {}
      H = nx.DiGraph()
      #for e in G.edges(G.nodes(), True, True):
	#u, v, i, d = e
      fifos = self.fifos
      for e in G.edges():
	print e
        u, v = e
        d = G.get_edge_data(u, v)
	w = sum([vv['weight'] for vv in d.values()])
	print u, v, d, w
	H.add_edge(u,v,weight=w)
       # edge_labels={(nodes[0],nodes[1]):'jj'})
      edges = H.edges(H.nodes(), True)
      print edges
      edge_weights = {}
      for u, v, d in edges:
      #for e in edges:
      #  print e
        e = (u, v)
	w = d['weight']
	er = (v, u)
	if er in edge_weights:
	  wr = edge_weights[er]
	  if v in fifos:
	    ws = "a-->f: %.1f  f-->a: %s" % (w, wr)
	  else:
	    ws = "a-->f: %s  f-->a: %.1f" % (wr, w)
	  edge_weights[er] = ws
	else:
	  edge_weights[e] = "%.1f" % w
      print edge_weights
      return edge_weights

  def draw_twocolor_(self, G, show_labels=False, plotfile="", show = False, figsize=(30,30)):
    plt.figure(figsize=figsize)

    nodes = G.nodes()
    ns = set(nodes)
    atoms = ns.intersection(set(self.atoms))
    fifos = ns.intersection(set(self.fifos))

    labels = {}

    if show_labels:
      for n in nodes:
	labels[n] = n

    #pos=nx.spring_layout(G) # positions for all nodes
    #pos=nx.spectral_layout(G) # positions for all nodes
    pos=nx.circular_layout(G) # positions for all nodes

    # nodes
    nx.draw_networkx_nodes(G,pos,
			   nodelist=atoms,
			   node_color='r',
			   node_size=1000,
		       alpha=0.5)
    nx.draw_networkx_nodes(G,pos,
			   nodelist=fifos,
			   node_color='b',
			   node_size=1000,
		       alpha=0.5)

    # edges
    nx.draw_networkx_edges(G,pos,width=1.0,alpha=0.5)
#    nx.draw_networkx_edges(G,pos,
#			   edgelist=[(0,1),(1,2),(2,3),(3,0)],
#			   width=8,alpha=0.5,edge_color='r')
#    nx.draw_networkx_edges(G,pos,
#			   edgelist=[(4,5),(5,6),(6,7),(7,4)],
#			   width=8,alpha=0.5,edge_color='b')
    if labels:
      #nx.draw_networkx_labels(G,pos,labels,font_size=16)
      nx.draw_networkx_labels(G,pos,labels,font_size=10)

      edge_labels = self.flattened_edge_weights(G)
      nx.draw_networkx_edge_labels(G,pos,edge_labels)
     # edge_labels={(nodes[0],nodes[1]):'jj'})

    plt.axis('off')
    if plotfile:
      plt.savefig(plotfile) # save as png
    if show:
      plt.show()

  def draw_twocolor(self):
    G = self.P
    plotfile = "__program.png"
    self.draw_twocolor_(G, False, plotfile)


  #---------------------------------
  # constructor
  #---------------------------------
  def __init__(self, logger):
    self.logger = logger

    #top-level program of the app
    P = nx.MultiDiGraph()
    self.P = P

class R1CodeGenerator:

  def gencode_r1(self, pc):
    #sys.exit(1)
    pass

  def __init__(self, logger):
    self.logger = logger
