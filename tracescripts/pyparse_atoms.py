'''
Atomix project, pyparse_atoms.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sys, os
from pyparse_util import *
import matplotlib
import matplotlib.pyplot as plt
import pdb
FIGSIZE_PLOT_ATOMS = (70,70)

def parse_atomis_single_without_fifo_wait_profiling(dd):
  dd_fi = dd[0::4]
  dd_ii = dd[1::4]
  dd_io = dd[2::4]
  dd_fo = dd[3::4]

  durs_fg = {}
  durs_ii = {}
  durs_fd = {}
  for (idv1,ts1),(idv2,ts2),(idv3,ts3),(idv4,ts4) in zip(dd_fi,dd_ii,dd_io,dd_fo):
    t_fg = ts2 - ts1
    t_ii = ts3 - ts2
    t_fd = ts4 - ts3
    if len(set([idv1,idv2,idv3,idv4])) != 1:
      print 'ERROR: id mismatch'
      sys.exit(1)
    if idv1 in durs_fg:
	durs_fg[idv1].append(t_fg)
	durs_ii[idv1].append(t_ii)
	durs_fd[idv1].append(t_fd)
    else:
        durs_fg[idv1] = [t_fg]
        durs_ii[idv1] = [t_ii]
        durs_fd[idv1] = [t_fd]
      
  stats_fg = {}
  stats_ii = {}
  stats_fd = {}
  for atomid in durs_fg:
    stats_fg[atomid] = stats(durs_fg[atomid])
    stats_ii[atomid] = stats(durs_ii[atomid])
    stats_fd[atomid] = stats(durs_fd[atomid])


  return stats_fg, stats_ii, stats_fd, durs_fg, durs_ii, durs_fd

def parse_atomis_single(dd, conf):		#with fifo wait profile information included

  in_a_pair = False
  atom_ts_order = 0
  fifo_wait_time = 0
  durs_fw = {}
  dd_no_wait_ts = []

  #for idv, ts in dd[1:100]:
  #  print idv,ts
  #raw_input()

  cntr = 1
  for (idv,ts) in dd:
    ts = ts - cntr * TS_OVERHEAD
    cntr += 1
    if idv != fifo_wait_ts_id:
      dd_no_wait_ts.append((idv, ts))

      atom_ts_order = (atom_ts_order + 1) % 4

      if atom_ts_order == 2:
	if idv in durs_fw:
	  durs_fw[idv].append(fifo_wait_time)
	else:
	  durs_fw[idv] = [fifo_wait_time]

	fifo_wait_time = 0
	if in_a_pair:
	  print("ERROR: inconsistent fifo wait trace")
	  sys.exit(1)

      continue

    else:	#fifo_wait_ts_id
      if atom_ts_order == 0:
	continue

      if atom_ts_order != 1:
        print ("ERROR: inconsistent fifo wait trace - not expected")
	sys.exit(1)

      if not in_a_pair:
	in_a_pair = True
	tin = ts
      else:
        in_a_pair = False
        tout = ts
	twait = tout - tin
	fifo_wait_time += twait

  dd = dd_no_wait_ts
      
  dd_fi = dd[0::4]
  dd_ii = dd[1::4]
  dd_io = dd[2::4]
  dd_fo = dd[3::4]

  durs_fg = {}
  durs_ii = {}
  durs_fd = {}
  for (idv1,ts1),(idv2,ts2),(idv3,ts3),(idv4,ts4) in zip(dd_fi,dd_ii,dd_io,dd_fo):
    t_fg = ts2 - ts1
    t_ii = ts3 - ts2
    t_fd = ts4 - ts3
    if len(set([idv1,idv2,idv3,idv4])) != 1:
      print 'ERROR: id mismatch'
      sys.exit(1)
    if idv1 in durs_fg:
	durs_fg[idv1].append(t_fg)
	durs_ii[idv1].append(t_ii)
	durs_fd[idv1].append(t_fd)
    else:
        durs_fg[idv1] = [t_fg]
        durs_ii[idv1] = [t_ii]
        durs_fd[idv1] = [t_fd]

  durs_fgmw = {}	#fg - fw
  for idv in durs_fg:
    durs_fgmw[idv] = [x - y for x, y in zip(durs_fg[idv], durs_fw[idv])]
      
  stats_fg = {}
  stats_ii = {}
  stats_fd = {}
  stats_fw = {}
  stats_fgmw = {}
  for atomid in durs_fg:
    stats_fg[atomid] = stats(durs_fg[atomid])
    stats_ii[atomid] = stats(durs_ii[atomid])
    stats_fd[atomid] = stats(durs_fd[atomid])
    stats_fw[atomid] = stats(durs_fw[atomid])
    stats_fgmw[atomid] = stats(durs_fgmw[atomid])


  return stats_fg, stats_ii, stats_fd, stats_fw, stats_fgmw, durs_fg, durs_ii, durs_fd, durs_fw, durs_fgmw


def parse_atoms_single_atii(dd, matched_ts_pair=False):
  #ddin = dd[:-1]
  #ddout = dd[1:]

  if matched_ts_pair:
    id_diff = 0
    ddin = dd[:-1:2]
    ddout = dd[1::2]
  else:
    id_diff = 1
    ddin = dd[:-1]
    ddout = dd[1:]

  durs = {}
  #for r in zip(ddin, ddout):
  #  print r
  #raw_input()
  atid_transition_sequence = []
  for (idv1,ts1),(idv2,ts2) in zip(ddin, ddout):
    tdiff = ts2 - ts1
    if int(idv2) - int(idv1) == id_diff:
      if idv1 in durs:
	durs[idv1].append(tdiff)
	#print idv1, tdiff, ts2, ts1
	#raw_input()
      else:
        durs[idv1] = [tdiff]
    atid_transition_sequence.append((int(idv1),(ts1,ts2)))
      
  all_stats = {}
  for atomid in durs:
    all_stats[atomid] = stats(durs[atomid])

  return all_stats, durs, atid_transition_sequence

def parse_atoms_single(dd, matched_ts_pair=False, conf={}):
  print conf
  TS_OVERHEAD = conf['TS_OVERHEAD']
  fifo_wait_ts_id, _ = conf['fifo_wait_ts_ids']

  durs = {}
  if not matched_ts_pair:
	  in_a_pair = False
	  #atom_ts_order = 0
	  fifo_wait_time = 0
	  durs_fw = {}
	  dd_no_wait_ts = []

	  #for idv, ts in dd[1:100]:
	  #  print idv,ts

	  very_first = True
	  tsp = 0

	  cntr = 1
	  atom_ts_idv = '0'
	  for (idv,ts) in dd:
	    ts = ts - cntr * TS_OVERHEAD
	    cntr += 1
	    if idv != fifo_wait_ts_id:

	#      if atom_ts_idv == '103201':
	#	print atom_ts_idv, ts, tsp, ts - tsp, fifo_wait_time


	      if not very_first:
		dd_no_wait_ts.append((idv, ts))


	      if atom_ts_idv != '0' and not very_first:

		#####################
		if atom_ts_idv in durs:
		  durs[atom_ts_idv].append(ts - tsp)
		else:
		  durs[atom_ts_idv] = [ts - tsp]
		#####################


		if atom_ts_idv in durs_fw:
		  durs_fw[atom_ts_idv].append(fifo_wait_time)
		else:
		  durs_fw[atom_ts_idv] = [fifo_wait_time]

	      fifo_wait_time = 0
	      if in_a_pair:
		print("ERROR: inconsistent fifo wait trace")
		sys.exit(1)

	      tsp = ts
	      atom_ts_idv = idv
	      very_first = False

	    else:	#fifo_wait_ts_id
	      if atom_ts_idv == '0':
		continue

	      if not in_a_pair:
		in_a_pair = True
		tin = ts
	      else:
		in_a_pair = False
		tout = ts
		twait = tout - tin
		fifo_wait_time += twait

  else:
    print 'ERROR: matched_ts_pair implementation is incomplete.'
    sys.exit(1)

  dd = dd_no_wait_ts

  #for idv, ts in dd[0:10]:
  #  print idv,ts

  ####ddin = dd[:-1]
  ####ddout = dd[1:]


  ####if matched_ts_pair:
  ####  id_diff = 0
  ####else:
  ####  id_diff = 1

  ####durs = {}
  ####for (idv1,ts1),(idv2,ts2) in zip(ddin, ddout):
  ####  tdiff = ts2 - ts1
  ####  if int(idv2) - int(idv1) == id_diff:
  ####    if idv1 in durs:
  ####      durs[idv1].append(tdiff)
  ####    else:
  ####      durs[idv1] = [tdiff]
      

  #remove fifo wait delays from calculation
  for idv in durs:
    durs[idv] = [x - y for x, y in zip(durs[idv], durs_fw[idv])]


  all_stats = {}
  for atomid in durs:
    all_stats[atomid] = stats(durs[atomid])

  return all_stats, durs


def parse_atomis(tf, ddtup, conf):
  (dd0, dd1, dd2, dd3) = ddtup

  tfb = tf.split('.')[0]
  atom_table_f = '%s_atom_profiling_table.txt' % tfb
  f = open(atom_table_f,'r')
  atom_labels={}
  for line in f.readlines():
    (atomid,wblockname,atomname) = [x.strip() for x in line.split(',')]
    label = '%s(%s)' % (wblockname,atomname)
    atom_labels[atomid] = label
    #print (atomid, label)
  f.close()

  stats_fg_0, stats_ii_0, stats_fd_0, stats_fw_0, stats_fgmw_0, durs_fg_0, durs_ii_0, durs_fd_0, durs_fw_0, durs_fgmw_0 = parse_atomis_single(dd0, conf)
  stats_fg_1, stats_ii_1, stats_fd_1, stats_fw_1, stats_fgmw_1, durs_fg_1, durs_ii_1, durs_fd_1, durs_fw_1, durs_fgmw_1 = parse_atomis_single(dd1, conf)
  stats_fg_2, stats_ii_2, stats_fd_2, stats_fw_2, stats_fgmw_2, durs_fg_2, durs_ii_2, durs_fd_2, durs_fw_2, durs_fgmw_2 = parse_atomis_single(dd2, conf)
  stats_fg_3, stats_ii_3, stats_fd_3, stats_fw_3, stats_fgmw_3, durs_fg_3, durs_ii_3, durs_fd_3, durs_fw_3, durs_fgmw_3 = parse_atomis_single(dd3, conf)

  #########################################
  #filter by those we want to plot
  #########################################
  atoms_to_plot_table_f = '%s_atoms_to_plot.txt' % tfb
  f = open(atoms_to_plot_table_f,'r')
  atoms_to_plot_labels={}
  for line in f.readlines():
    if line.startswith('#'):
      continue
    (atomid,wblockname,atomname) = [x.strip() for x in line.split(',')]
    label = '%s(%s)' % (wblockname,atomname)
    atoms_to_plot_labels[atomid] = label
    #print (atomid, label)
  f.close()

  stats_fg_0_f = {}; durs_fg_0_f = {} 
  stats_fg_1_f = {}; durs_fg_1_f = {} 
  stats_fg_2_f = {}; durs_fg_2_f = {} 
  stats_fg_3_f = {}; durs_fg_3_f = {} 
  stats_ii_0_f = {}; durs_ii_0_f = {} 
  stats_ii_1_f = {}; durs_ii_1_f = {} 
  stats_ii_2_f = {}; durs_ii_2_f = {} 
  stats_ii_3_f = {}; durs_ii_3_f = {} 
  stats_fd_0_f = {}; durs_fd_0_f = {} 
  stats_fd_1_f = {}; durs_fd_1_f = {} 
  stats_fd_2_f = {}; durs_fd_2_f = {} 
  stats_fd_3_f = {}; durs_fd_3_f = {} 
  stats_fw_0_f = {}; durs_fw_0_f = {} 
  stats_fw_1_f = {}; durs_fw_1_f = {} 
  stats_fw_2_f = {}; durs_fw_2_f = {} 
  stats_fw_3_f = {}; durs_fw_3_f = {} 

  for atomid in atoms_to_plot_labels:
    if atomid in stats_fg_0:
      stats_fg_0_f[atomid] = stats_fg_0[atomid]; durs_fg_0_f[atomid] = durs_fg_0[atomid]
      stats_ii_0_f[atomid] = stats_ii_0[atomid]; durs_ii_0_f[atomid] = durs_ii_0[atomid]
      stats_fd_0_f[atomid] = stats_fd_0[atomid]; durs_fd_0_f[atomid] = durs_fd_0[atomid]
      stats_fw_0_f[atomid] = stats_fw_0[atomid]; durs_fw_0_f[atomid] = durs_fw_0[atomid]
    if atomid in stats_fg_1:
      stats_fg_1_f[atomid] = stats_fg_1[atomid]; durs_fg_1_f[atomid] = durs_fg_1[atomid]
      stats_ii_1_f[atomid] = stats_ii_1[atomid]; durs_ii_1_f[atomid] = durs_ii_1[atomid]
      stats_fd_1_f[atomid] = stats_fd_1[atomid]; durs_fd_1_f[atomid] = durs_fd_1[atomid]
      stats_fw_1_f[atomid] = stats_fw_1[atomid]; durs_fw_1_f[atomid] = durs_fw_1[atomid]
    if atomid in stats_fg_2:
      stats_fg_2_f[atomid] = stats_fg_2[atomid]; durs_fg_2_f[atomid] = durs_fg_2[atomid]
      stats_ii_2_f[atomid] = stats_ii_2[atomid]; durs_ii_2_f[atomid] = durs_ii_2[atomid]
      stats_fd_2_f[atomid] = stats_fd_2[atomid]; durs_fd_2_f[atomid] = durs_fd_2[atomid]
      stats_fw_2_f[atomid] = stats_fw_2[atomid]; durs_fw_2_f[atomid] = durs_fw_2[atomid]
    if atomid in stats_fg_3:
      stats_fg_3_f[atomid] = stats_fg_3[atomid]; durs_fg_3_f[atomid] = durs_fg_3[atomid]
      stats_ii_3_f[atomid] = stats_ii_3[atomid]; durs_ii_3_f[atomid] = durs_ii_3[atomid]
      stats_fd_3_f[atomid] = stats_fd_3[atomid]; durs_fd_3_f[atomid] = durs_fd_3[atomid]
      stats_fw_3_f[atomid] = stats_fw_3[atomid]; durs_fw_3_f[atomid] = durs_fw_3[atomid]

  stats_fg_0 = stats_fg_0_f; durs_fg_0 = durs_fg_0_f
  stats_fg_1 = stats_fg_1_f; durs_fg_1 = durs_fg_1_f
  stats_fg_2 = stats_fg_2_f; durs_fg_2 = durs_fg_2_f
  stats_fg_3 = stats_fg_3_f; durs_fg_3 = durs_fg_3_f
  stats_ii_0 = stats_ii_0_f; durs_ii_0 = durs_ii_0_f
  stats_ii_1 = stats_ii_1_f; durs_ii_1 = durs_ii_1_f
  stats_ii_2 = stats_ii_2_f; durs_ii_2 = durs_ii_2_f
  stats_ii_3 = stats_ii_3_f; durs_ii_3 = durs_ii_3_f
  stats_fd_0 = stats_fd_0_f; durs_fd_0 = durs_fd_0_f
  stats_fd_1 = stats_fd_1_f; durs_fd_1 = durs_fd_1_f
  stats_fd_2 = stats_fd_2_f; durs_fd_2 = durs_fd_2_f
  stats_fd_3 = stats_fd_3_f; durs_fd_3 = durs_fd_3_f
  stats_fw_0 = stats_fw_0_f; durs_fw_0 = durs_fw_0_f
  stats_fw_1 = stats_fw_1_f; durs_fw_1 = durs_fw_1_f
  stats_fw_2 = stats_fw_2_f; durs_fw_2 = durs_fw_2_f
  stats_fw_3 = stats_fw_3_f; durs_fw_3 = durs_fw_3_f

  atom_labels = atoms_to_plot_labels
  ###################################



  atomi_stats_fg = []
  atomi_stats_ii = []
  atomi_stats_fd = []
  atomi_stats_fw = []
  atomi_stats_fgmw = []
  atomi_stats_fg.extend(stats_fg_0.items())
  atomi_stats_fg.extend(stats_fg_1.items())
  atomi_stats_fg.extend(stats_fg_2.items())
  atomi_stats_fg.extend(stats_fg_3.items())
  atomi_stats_ii.extend(stats_ii_0.items())
  atomi_stats_ii.extend(stats_ii_1.items())
  atomi_stats_ii.extend(stats_ii_2.items())
  atomi_stats_ii.extend(stats_ii_3.items())
  atomi_stats_fd.extend(stats_fd_0.items())
  atomi_stats_fd.extend(stats_fd_1.items())
  atomi_stats_fd.extend(stats_fd_2.items())
  atomi_stats_fd.extend(stats_fd_3.items())
  atomi_stats_fw.extend(stats_fw_0.items())
  atomi_stats_fw.extend(stats_fw_1.items())
  atomi_stats_fw.extend(stats_fw_2.items())
  atomi_stats_fw.extend(stats_fw_3.items())
  atomi_stats_fgmw.extend(stats_fgmw_0.items())
  atomi_stats_fgmw.extend(stats_fgmw_1.items())
  atomi_stats_fgmw.extend(stats_fgmw_2.items())
  atomi_stats_fgmw.extend(stats_fgmw_3.items())

  durs_fg = []
  durs_ii = []
  durs_fd = []
  durs_fw = []
  durs_fgmw = []
  durs_fg.extend(durs_fg_0.items())
  durs_fg.extend(durs_fg_1.items())
  durs_fg.extend(durs_fg_2.items())
  durs_fg.extend(durs_fg_3.items())
  durs_ii.extend(durs_ii_0.items())
  durs_ii.extend(durs_ii_1.items())
  durs_ii.extend(durs_ii_2.items())
  durs_ii.extend(durs_ii_3.items())
  durs_fd.extend(durs_fd_0.items())
  durs_fd.extend(durs_fd_1.items())
  durs_fd.extend(durs_fd_2.items())
  durs_fd.extend(durs_fd_3.items())
  durs_fw.extend(durs_fw_0.items())
  durs_fw.extend(durs_fw_1.items())
  durs_fw.extend(durs_fw_2.items())
  durs_fw.extend(durs_fw_3.items())
  durs_fgmw.extend(durs_fgmw_0.items())
  durs_fgmw.extend(durs_fgmw_1.items())
  durs_fgmw.extend(durs_fgmw_2.items())
  durs_fgmw.extend(durs_fgmw_3.items())



  atomi_labels_dict = atom_labels
  atomi_stats = (atomi_stats_fg, atomi_stats_ii, atomi_stats_fd, atomi_stats_fw, atomi_stats_fgmw)
  atomi_durs = (durs_fg, durs_ii, durs_fd, durs_fw, durs_fgmw)
  return atomi_stats, atomi_durs, atomi_labels_dict  #same_order

def parse_time_spent(tf, ddtup, dd_runs):
  #(dd0, dd1, dd2, dd3) = ddtup
  xs = {}
  for run in dd_runs:
    dd = dd_runs[run]

    dd0 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '0']
    dd1 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '1']
    dd2 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '2']
    dd3 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '3']

    time_spent_0 =  parse_time_spent_single(dd0, True)
    time_spent_1 =  parse_time_spent_single(dd1, True)
    time_spent_2 =  parse_time_spent_single(dd2, True)
    time_spent_3 =  parse_time_spent_single(dd3, True)
    
    time_spent = {}
    time_spent.update(time_spent_0)
    time_spent.update(time_spent_1)
    time_spent.update(time_spent_2)
    time_spent.update(time_spent_3)

    x = time_spent
    sorted_x = sorted(x.iteritems(), key=operator.itemgetter(1))

    xs[run] = sorted_x
    #for row in sorted_x:
    #  print row

  allxs = xs.values()
  n_runs = len(allxs)
  n_vals = len(allxs[0])
  for i in range(n_vals):
    for j in range(min(n_runs, 15)):
      try:
	print allxs[j][i],"\t",
      except IndexError:
      	print "\t",
        pass
    print

  #return (ids, time_spent)
  #return x

def parse_time_spent_single(dd, matched_ts_pair=False):
  ddin = dd[:-1]
  ddout = dd[1:]

  if matched_ts_pair:
    id_diff = 0
  else:
    id_diff = 1

  durs = {}
  time_spent = {}
  for (idv1,ts1),(idv2,ts2) in zip(ddin, ddout):
    tdiff = ts2 - ts1
    if int(idv2) - int(idv1) == id_diff:
      if idv1 in durs:
	durs[idv1].append(tdiff)
	time_spent[idv1] += tdiff
      else:
        durs[idv1] = [tdiff]
	time_spent[idv1] = tdiff
      

  return time_spent


def classified_util(ddtup, conf, atom_classification_table_file, atom_labels):
  entry_atom = conf['pkt_entry_atom']
  exit_atom = conf['pkt_exit_atom']
  entry_atomid = [atid for atid, lbl in atom_labels.items() \
      if lbl.split("(")[1].rstrip(")") == entry_atom][0]
  exit_atomid = [atid for atid, lbl in atom_labels.items() \
      if lbl.split("(")[1].rstrip(")") == exit_atom][0]
  print entry_atom, entry_atomid, exit_atom, exit_atomid

  entry_tups = []
  exit_tups = []
  for dd in ddtup:
    #print dd
    entry_tups += [(atid, ts) for atid, ts in dd if atid == entry_atomid]
    exit_tups += [(atid, ts) for atid, ts in dd if atid == exit_atomid]
  entry_cyc = min([ts for _, ts in entry_tups])
  exit_cyc = min([ts for _, ts in exit_tups])
  pkt_dur_cyc = exit_cyc - entry_cyc

  print entry_tups, exit_tups
  print entry_cyc, exit_cyc
  print pkt_dur_cyc

  pkt_ddtup = []
  #keep only the log that lies within pkt time stamps
  #also throw out trace reader special timestamps because we don't need them
  trin = conf['traceReaderInTSID']
  trout = conf['traceReaderOutTSID']
  for dd in ddtup:
    pkt_dd = [(i, (atid, ts)) for i, (atid, ts) in enumerate(dd) \
    		if ts >= entry_cyc and ts <= exit_cyc and atid != trin and atid != trout]
    pkt_ddtup.append(pkt_dd)
  pkt_ddtup = tuple(pkt_ddtup)

  pdd = []
  core = 0
  for dd in pkt_ddtup:
    print "core", core
    t = dd[0]
    (i, (atid, ts)) = t
    if i % 2 != 0:	
      #this means there is an operation that we have account for which splits
      #across the pkt boundary; we want to truncate it to be accounted
      #correctly within pkt boundary
      print 'odd index at start point: ', t
      #print 'next tuple: ', dd[1]
      atidb = int(atid)
      if atidb >= 1000:
        atidb = "%d" % (atidb - 1)
      else:
        atidb = "%d" % (atidb)
      tsb = entry_cyc
      tb = (i-1, (atidb, tsb))
      print 'inserting even index from pkt boundary: ', tb
      dd = [tb] + dd
      print 'first two tuples after insertion:', dd[0], dd[1]
    t = dd[-1]
    (i, (atid, ts)) = t
    if i % 2 == 0:
      #this means there is an operation that we have account for which splits
      #across the pkt boundary; we want to truncate it to be accounted
      #correctly within pkt boundary
      print 'even index at end point: ', t
      #print 'previous tuple: ', dd[-2]
      atidb = int(atid)
      if atidb >= 1000:
        atidb = "%d" % (atidb - 1)
      else:
        atidb = "%d" % (atidb)
      tsb = exit_cyc
      tb = (i+1, (atidb, tsb))
      print 'inserting odd index to pkt boundary: ', tb
      dd = dd + [tb]
      print 'last two tuples after insertion:', dd[-2], dd[-1]
    pdd.append(dd)

    core += 1

  pkt_ddtup = []
  for dd in pdd:
    dd = [(atid, ts) for i, (atid, ts) in dd]
    pkt_ddtup.append(dd)
  pkt_ddtup = tuple(pkt_ddtup)

  #print pkt_ddtup[1]
  #print pdd[1]
  #sys.exit(1)

  atom_table_f = atom_classification_table_file
  f = open(atom_table_f,'r')
  atom_labels={}
  for line in f.readlines():
    (classlbl,atomid,wblockname,atomname) = [x.strip() for x in line.split(',')]
    #label = '%s(%s)' % (wblockname,atomname)
    label = '%s,%s,%s' % (classlbl,wblockname,atomname)
    atom_labels[atomid] = label
    #print (atomid, label)
  f.close()

  classified_cycles_spent = {}
  classes = set([lb.split(',')[0] for lb in atom_labels.values()])
  for classid in classes:
    classified_cycles_spent[classid] = []
  classified_cycles_spent['ww'] = []

  jjs = []
  core = 0
  for dd in pkt_ddtup:
    dd = zip(range(0,len(dd)), dd)
    dd_atoms = [(i,(atid, ts)) for (i,(atid, ts)) in dd if int(atid) < 1000]
    dd_specials = [(i,(atid, ts)) for (i,(atid, ts)) in dd if int(atid) >= 1000]
    #print dd_specials
    print "len_dd_specials = ", len(dd_specials)
    #pp = zip(dd[0::2],dd[1::2])
    pp = zip(dd_atoms[0::2],dd_atoms[1::2])
    #for p in pp:
    #  print p
    #print dd_specials
    ss = zip(dd_specials[0::2],dd_specials[1::2])
    #for s in ss:
    #  print s

    it = iter(ss)
    jj = []
    s = next(it, ((0, ('0', 0)), (0, ('0', 0))))

    for p in pp:
      (pidxin, (atid1,pcyc1)), (pidxout, (atid2,pcyc2)) = p
      pdelta = pcyc2 - pcyc1
      if atid1 != atid2:
        print "ERROR: inconsistency detected: atom ids do not match as needed (+0)"
	sys.exit(1)
      atid = atid1
      (sidxin, (atid1,scyc1)), (sidxout, (atid2,scyc2)) = s
      sdelta = scyc2 - scyc1
      if int(atid1) and int(atid2) and int(atid2) != int(atid1) + 1:
        print "ERROR: inconsistency detected: atom ids do not match as needed (+1)"
	print s, "core = ", core
	#for t in dd:
	#  print t
	sys.exit(1)

      #pure waits (not within atii) before atom
      while sidxout and sidxout < pidxin:
	#print pidxin, pidxout, sidxin, sidxout
        jj.append(('ww,FF,ff', s, []))
	s = next(it, ((0, ('0', 0)), (0, ('0', 0))))
	(sidxin, (atid1,scyc1)), (sidxout, (atid2,scyc2)) = s
	sdelta = scyc2 - scyc1
	if int(atid1) and int(atid2) and int(atid2) != int(atid1) + 1:
	  print "ERROR: inconsistency detected: atom ids do not match as needed (+1)"
	  print s, "core = ", core
	  #for t in dd:
	  #  print t
	  sys.exit(1)
	classified_cycles_spent['ww'].append(sdelta)

      specials = []
      wdelta = 0

      #contained waits in this atom
      while sidxout and pidxin < sidxin and sidxout < pidxout:
	#print pidxin, pidxout, sidxin, sidxout
	specials.append(s)
	wdelta += sdelta
	pdelta -= sdelta
	s = next(it, ((0, ('0', 0)), (0, ('0', 0))))
	(sidxin, (atid1,scyc1)), (sidxout, (atid2,scyc2)) = s
	sdelta = scyc2 - scyc1
	if int(atid1) and int(atid2) and int(atid2) != int(atid1) + 1:
	  print "ERROR: inconsistency detected: atom ids do not match as needed (+1)"
	  print s, "core = ", core
	  #for t in dd:
	  #  print t
	  sys.exit(1)

      jj.append((atom_labels[atid], p, specials))
      classid = atom_labels[atid].split(',')[0]

      classified_cycles_spent[classid].append(pdelta)
      classified_cycles_spent['ww'].append(wdelta)

    #pure waits (not within atii) after the last atom
    while sidxout and sidxin > pidxin:
      #print pidxin, pidxout, sidxin, sidxout
      jj.append(('ww,FF,ff', s, []))
      s = next(it, ((0, ('0', 0)), (0, ('0', 0))))
      (sidxin, (atid1,scyc1)), (sidxout, (atid2,scyc2)) = s
      sdelta = scyc2 - scyc1
      if int(atid1) and int(atid2) and int(atid2) != int(atid1) + 1:
	print "ERROR: inconsistency detected: atom ids do not match as needed (+1)"
	print s, "core = ", core
	#for t in dd:
	#  print t
	sys.exit(1)
      classified_cycles_spent['ww'].append(sdelta)



    #print "core ", core
    #print '------'
    #for j in jj:
    #  print j
    #print '------'
    #print "core ", core

    jjs.append(jj)
    core += 1

  print "========="
  #print classified_cycles_spent
  ncores = len(ddtup)
  total_dur_cyc = pkt_dur_cyc * ncores
  total_cycles = {}
  print "len_ww_nz = ", len([dt for dt in classified_cycles_spent['ww'] if dt > 0.1])
  print "========="

  outfile = '__classified_usage_stats.txt'
  f = open(outfile, 'w')

  print 'all cores: aggregate classified usage'
  for classid in classified_cycles_spent:
    n = sum(classified_cycles_spent[classid])
    p = 100.0 * n/total_dur_cyc
    total_cycles[classid] = (n, p)
    print >>f, "allcores,%s,%10d, %4.1f" % (classid,int(n),p)
  classid = 's*'
  sigman = sum([n for (n, _) in total_cycles.values()])
  n = total_dur_cyc - sigman
  p = 100.0 * n/total_dur_cyc
  total_cycles[classid] = (n, p)
  print >>f, "allcores,%s,%10d, %4.1f" % (classid,int(n),p)
  print total_cycles
  print 'sum_total_cycles =', sum([n for (n, p) in total_cycles.values()])
  print 'total_dur_cycles = ', total_dur_cyc
  print "pkt_dur_cyc =", pkt_dur_cyc, "pkt_in_cyc =", entry_cyc, "pkt_out_cyc = ", exit_cyc
    
  #per-core classified utilization
  print "========="
  core = 0
  for jj in jjs:
    classified_cycles_spent = {}
    classified_cycles_spent['ww'] = []
    for t in jj:
      lb, ((_, (_, cyin)), (_, (_, cyout))), warray = t
      pdelta = cyout - cyin
      classid = lb.split(',')[0]
      if classid not in classified_cycles_spent:
        classified_cycles_spent[classid] = []
      wdelta = 0
      if warray:
        #warray = zip(warray[0::2], warray[1::2])
	wdelta = sum([cyout - cyin for ((_, (_, cyin)), (_,(_, cyout))) in warray])
      pdelta -= wdelta
      classified_cycles_spent[classid].append(pdelta)
      classified_cycles_spent['ww'].append(wdelta)
    total_cycles = {}
    total_dur_cyc = pkt_dur_cyc
    print "core", core
    for classid in classified_cycles_spent:
      n = sum(classified_cycles_spent[classid])
      p = 100.0 * n/total_dur_cyc
      total_cycles[classid] = (n, p)
      print >>f, "core%4d,%s,%10d, %4.1f" % (core,classid,int(n),p)
    classid = 's*'
    sigman = sum([n for (n, _) in total_cycles.values()])
    n = total_dur_cyc - sigman
    p = 100.0 * n/total_dur_cyc
    total_cycles[classid] = (n, p)
    print >>f, "core%4d,%s,%10d, %4.1f" % (core,classid,int(n),p)
    print total_cycles
    #  print t
    print "---------"
    #raw_input()
    core += 1

  f.close()
  sys.exit(1)


def parse_atoms(tf, ddtup, matched_ts_pair=False, is_atii=False, atom_profiling_table_file="atom_profiling_table.txt", atoms_to_plot_table_f="", conf={}, atom_classification_table_file=""):
  (dd0, dd1, dd2, dd3) = ddtup

  tfb = tf.split('.')[0]
  atom_table_f = atom_profiling_table_file
  f = open(atom_table_f,'r')
  atom_labels={}
  for line in f.readlines():
    (atomid,wblockname,atomname) = [x.strip() for x in line.split(',')]
    label = '%s(%s)' % (wblockname,atomname)
    atom_labels[atomid] = label
    #print (atomid, label)
  f.close()

  ############################################
  #processing for classified utilization stats
  ############################################
  if atom_classification_table_file:
    if not is_atii or not matched_ts_pair:
      print "classified_util only implemented for atii with matched_ts_pair"
      sys.exit(1)
    classified_util(ddtup, conf, atom_classification_table_file, atom_labels)

    #so durs_0, durs_1, durs_2, durs_3 have the actual run time for each recorded instance
    #atom_stats_x have the stats
    #the part after that is handling atom names and stuff
    #just print out durs_0
    #it's structured as {'atomid': [....], ...}
    #where atomid is the key in atom_profiling_table

  if is_atii:
    atom_stats_0, durs_0, atid_tseq_0 =  parse_atoms_single_atii(dd0, matched_ts_pair)
    atom_stats_1, durs_1, atid_tseq_1  =  parse_atoms_single_atii(dd1, matched_ts_pair)
    atom_stats_2, durs_2, atid_tseq_2  =  parse_atoms_single_atii(dd2, matched_ts_pair)
    atom_stats_3, durs_3, atid_tseq_3  =  parse_atoms_single_atii(dd3, matched_ts_pair)
  else:
    atom_stats_0, durs_0 =  parse_atoms_single(dd0, matched_ts_pair, conf)
    atom_stats_1, durs_1 =  parse_atoms_single(dd1, matched_ts_pair, conf)
    atom_stats_2, durs_2 =  parse_atoms_single(dd2, matched_ts_pair, conf)
    atom_stats_3, durs_3 =  parse_atoms_single(dd3, matched_ts_pair, conf)

  import pdb; pdb.set_trace()

  #########################################
  #filter by those we want to plot
  #########################################
  #atoms_to_plot_table_f = '%s_atoms_to_plot.txt' % tfb
  if not atoms_to_plot_table_f:
    atoms_to_plot_table_f = atom_profiling_table_file
  f = open(atoms_to_plot_table_f,'r')
  atoms_to_plot_labels={}
  for line in f.readlines():
    if line.startswith('#'):
      continue
    (atomid,wblockname,atomname) = [x.strip() for x in line.split(',')]
    if atomname == 'dummy':
      continue
    label = '%s(%s)' % (wblockname,atomname)
    atoms_to_plot_labels[atomid] = label
    #print (atomid, label)
  f.close()

  atom_stats_0_f = {}; durs_0_f = {} 
  atom_stats_1_f = {}; durs_1_f = {} 
  atom_stats_2_f = {}; durs_2_f = {} 
  atom_stats_3_f = {}; durs_3_f = {} 

  for atomid in atoms_to_plot_labels:
    if atomid in atom_stats_0:
      atom_stats_0_f[atomid] = atom_stats_0[atomid]; durs_0_f[atomid] = durs_0[atomid]
    if atomid in atom_stats_1:
      atom_stats_1_f[atomid] = atom_stats_1[atomid]; durs_1_f[atomid] = durs_1[atomid]
    if atomid in atom_stats_2:
      atom_stats_2_f[atomid] = atom_stats_2[atomid]; durs_2_f[atomid] = durs_2[atomid]
    if atomid in atom_stats_3:
      atom_stats_3_f[atomid] = atom_stats_3[atomid]; durs_3_f[atomid] = durs_3[atomid]
  atom_stats_0 = atom_stats_0_f; durs_0 = durs_0_f
  atom_stats_1 = atom_stats_1_f; durs_1 = durs_1_f
  atom_stats_2 = atom_stats_2_f; durs_2 = durs_2_f
  atom_stats_3 = atom_stats_3_f; durs_3 = durs_3_f

  atom_labels = atoms_to_plot_labels
  ###################################

  #########################################
  #plot raw data
  #########################################
  f = open("_raw_durations.txt", "w")
  for durs in [durs_0, durs_1, durs_2, durs_3]:
    for atomid in durs:
        for dur in durs[atomid]:
            f.write("%s %f\n" % (atoms_to_plot_labels[atomid], dur))
  f.close()
  
  #========================================

  atom_stats = []
  atom_stats.extend(atom_stats_0.items())
  atom_stats.extend(atom_stats_1.items())
  atom_stats.extend(atom_stats_2.items())
  atom_stats.extend(atom_stats_3.items())
  #sort by min of compute time
  atom_stats_sorted_min = sorted(atom_stats, key=lambda st: st[1][3])

  #sort by mean of compute time
  atom_stats_sorted_mean = sorted(atom_stats, key=lambda st: st[1][1])

  atom_dump_data(atom_stats_sorted_mean, atom_labels)

  #sort by standard deviation of compute time
  atom_stats_sorted_inc_std = sorted(atom_stats, key=lambda st: st[1][2])


  #sort by spread of compute time
  atom_stats_sorted_inc_diff = sorted(atom_stats, key=lambda st: st[1][5])

  for rowno, row in zip(range(len(atom_stats_sorted_inc_std)), atom_stats_sorted_inc_std):
    print rowno, row



  #--------------------------------------------
  atom_stats_sorted = atom_stats_sorted_min 

  print 'Sorted by chosen plotting sorting metric'
  #for rowno, row in zip(range(len(atom_stats_sorted)), atom_stats_sorted):
  for rowno, (atom_id, row) in zip(range(len(atom_stats_sorted)), atom_stats_sorted):
    print rowno, atom_id, atom_labels[atom_id], row

  print 'Sorted by inc std'
  for rowno, row in zip(range(len(atom_stats_sorted_inc_std)), atom_stats_sorted_inc_std):
    print rowno, row
  #--------------------------------------------

  print 'normalized atom stats'

  #normalize by mean
  #atom_stats_normalized = [(atom_id, tuple([x/atom_stats_one[1] for x in atom_stats_one])) for (atom_id, atom_stats_one) in atom_stats_sorted]

  #normalize by min
  atom_stats_normalized = [(atom_id, tuple([x/atom_stats_one[3] for x in atom_stats_one])) for (atom_id, atom_stats_one) in atom_stats_sorted]

  atom_stats_n = []
  for atom_stat_norm_one, atom_stat_one in zip(atom_stats_normalized, atom_stats_sorted):
    atom_id, (cnt, mean, std, minn, maxx, diff, median, pctl95) = atom_stat_norm_one
    ig, (cnto,_,_,_,_,_,_,_)  = atom_stat_one
    atom_stats_n.append((atom_id, (cnto, mean, std, minn, maxx, diff, median, pctl95)))
  atom_stats_normalized = atom_stats_n

  for rowno, row in zip(range(len(atom_stats_normalized)), atom_stats_normalized):
    print rowno, row

  atom_labels_sorted = []
  for atomid,_ in atom_stats_sorted:
    atom_labels_sorted.append(atom_labels[atomid])

  all_durs = {}
  all_durs.update(durs_0) 
  all_durs.update(durs_1) 
  all_durs.update(durs_2) 
  all_durs.update(durs_3)

  atom_labels_dict = atom_labels

  if is_atii:
    atid_tseqs = (atid_tseq_0, atid_tseq_1, atid_tseq_2, atid_tseq_3)
    return atom_stats_sorted, atom_labels_sorted, atom_stats_normalized, all_durs, atom_labels_dict, atid_tseqs  #same_order

  return atom_stats_sorted, atom_labels_sorted, atom_stats_normalized, all_durs, atom_labels_dict

def atom_dump_data(atom_stats,atom_labels):
  
  outfname = '__data_atom_durations.txt'
  f = open(outfname, 'w')
  
  atomids = [atomid for (atomid, stats) in atom_stats]
  stats = [stats for (atomid, stats) in atom_stats]
  means   = [mean   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stats]
  for (atomid, mean) in zip(atomids,means):
    lb = atom_labels[atomid]
    print >>f, "atomid=%s,lb=%s,mean=%s" % \
    	(atomid, lb, mean)

  print 'saved atom durations to', outfname

def plot_atoms_i_horz(tfname, atom_stats_sorted, atom_labels_sorted, tfname_labelmap):
  atomids = [atomid for (atomid, stat_tup) in atom_stats_sorted]
  stat_tups = [stat_tup for (atomid, stat_tup) in atom_stats_sorted]
  means   = [mean   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  medians = [median for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  mins    = [minn   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  maxs    = [maxx   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  pctls95 = [pctl95 for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]

  plt.figure()
  ####plt.margins(0.1)	#extend axis limits by this fraction
  ####plt.subplots_adjust(left=0.15, right=0.75, bottom=0.1, top=0.9)
  plt.subplots_adjust(left=0.15, right=0.9, bottom=0.1, top=0.85)
  plt.title('Atom compute times and predictability\n%s' % os.getcwd().split('orsys')[1])

  #plt.plot(c0_ts, c0_st, 'r', c1_ts, c1_st, 'g', c2_ts, c2_st, 'b', c3_ts, c3_st, 'k')
  ids = range(len(atomids))
  f = open(tfname_labelmap, 'w')
  for id_, atomid, label in zip(ids, atomids, atom_labels_sorted):
    f.write('%s,%s,%s\n' % (id_,atomid,label))
  f.close()

  plt.plot(ids, means   ,  'b-')
  plt.plot(ids, maxs    ,  'r+-')
  plt.plot(ids, mins    ,  'g.-')
  plt.plot(ids, medians ,  'k--')
  plt.plot(ids, pctls95 ,  'k-.')

  xs = ids
  #ys = maxs
  #ys = means
  ys = mins 
  ats = atom_labels_sorted
  #for x, y, at, atid in zip(xs, ys, ats, atomids):
#    #plt.annotate(at, xy=(x,y), rotation=60)
#    #plt.annotate(at, xy=(y*1.4,x), rotation=0, fontsize='x-small')
#    plt.annotate(atid, xy=(x, y), rotation=0, fontsize='x-small')
    ####plt.annotate(atid, xy=(x, y - 100), rotation=0, fontsize='x-small')
#
  #plt.xlabel('Action ID')
  #plt.ylabel('Cycles')
  plt.xlabel('Atom ID')
  plt.ylabel('Cycles')
  plt.grid(True)

  legendvals = ['mean', 'max', 'min', 'median','95%tile']

  #fig = plt.gcf()
  #fig.set_size_inches(18.5,10.5)
  #------
  #plt.legend(legendvals, loc='lower right')
  #------
  plt.legend(legendvals, loc='upper left')
  #------
  ## Shink current axis by 20%
  #ax = plt.subplot(111)
  #box = ax.get_position()
  #ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
  ## Put a legend to the right of the current axis
  ##plt.legend(legendvals)
  #ax.legend(legendvals, loc='center left', bbox_to_anchor=(1, 0.5))
  #------
##  ax = plt.subplot(111)
##  # Shink current axis's height by 10% on the bottom
##  box = ax.get_position()
##  ax.set_position([box.x0, box.y0 + box.height * 0.1,
##		   box.width, box.height * 0.9])
##  # Put a legend below current axis
##  #ax.legend(worst_top_labels, loc='upper center', bbox_to_anchor=(0.5, -0.05))#,
##  ax.legend(legendvals, loc='upper center', bbox_to_anchor=(0.5, -0.13),
##           fancybox=True, shadow=True, ncol=5)
##


  #plt.show()

  plt.savefig(tfname)
  #plt.savefig('%s_plot_states.png' % tf, pad_inches=2)


def plot_atoms_i(tfname, atom_stats_sorted, atom_labels_sorted):
  atomids = [atomid for (atomid, stat_tup) in atom_stats_sorted]
  stat_tups = [stat_tup for (atomid, stat_tup) in atom_stats_sorted]
  means   = [mean   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  medians = [median for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  mins    = [minn   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  maxs    = [maxx   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  pctls95 = [pctl95 for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]

  plt.figure(figsize=FIGSIZE_PLOT_ATOMS)
  plt.margins(0.1)	#extend axis limits by this fraction
  plt.subplots_adjust(left=0.15, right=0.75, bottom=0.1, top=0.85)
  plt.title('Atom compute times and predictability\n%s' % os.getcwd().split('orsys')[1])

  #plt.plot(c0_ts, c0_st, 'r', c1_ts, c1_st, 'g', c2_ts, c2_st, 'b', c3_ts, c3_st, 'k')
  ids = range(len(atomids))
  plt.plot(means   ,ids,  'b.-')
  plt.plot(maxs    ,ids,  'r.-')
  plt.plot(mins    ,ids,  'g.-')
  plt.plot(medians ,ids,  'k.--')
  plt.plot(pctls95 ,ids,  'k.-')

  xs = ids
  #ys = maxs
  ys = means
  ats = atom_labels_sorted
  for x, y, at, atid in zip(xs, ys, ats, atomids):
    #plt.annotate(at, xy=(x,y), rotation=60)
    ann = '%s, %s' % (y, at)
    #plt.annotate(ann, xy=(y*1.4,x), rotation=0, fontsize='x-small')
    plt.annotate(ann, xy=(y+20,x), rotation=0, fontsize='x-small')

  #plt.xlabel('Action ID')
  #plt.ylabel('Cycles')
  plt.ylabel('Atom ID')
  plt.xlabel('Cycles')
  plt.grid(True)

  legendvals = ['mean', 'max', 'min', 'median','95%tile']
  plt.legend(legendvals, loc='lower right')
  #------
  ax = plt.subplot(111)
  # Shink current axis's height by 10% on the bottom
  box = ax.get_position()
  ax.set_position([box.x0, box.y0 + box.height * 0.1,
		   box.width, box.height * 0.9])
  # Put a legend below current axis
  #ax.legend(worst_top_labels, loc='upper center', bbox_to_anchor=(0.5, -0.05))#,
  ax.legend(legendvals, loc='upper center', bbox_to_anchor=(0.5, -0.13),
           fancybox=True, shadow=True, ncol=5)



  #plt.show()
  plt.savefig(tfname)
  #plt.savefig('%s_plot_states.png' % tf, pad_inches=2)

def plot_atomis_i(tfname, atomi_stats, atomi_labels, atomi_durs):
  plot_atomis_i_one_figure(tfname, atomi_stats, atomi_labels, atomi_durs)
  plot_atomis_i_sep_figures(tfname, atomi_stats, atomi_labels, atomi_durs)

def plot_atomis_i_sep_figures(tfname, atomi_stats, atomi_labels, atomi_durs):
  atomi_stats_fg, atomi_stats_ii, atomi_stats_fd, atomi_stats_fw, atomi_stats_fgmw  = atomi_stats 
  atomi_stats_a = [atomi_stats_fg, atomi_stats_fw, atomi_stats_fgmw, atomi_stats_ii, atomi_stats_fd] 

  #plt.figure(figsize=(20,10))
  #plt.figure(figsize=(50,20))

  legendvals = ['mean', 'max', 'min', 'median','95%tile']
  titles = ['FIFO get buffer times', 'FIFO_wait_times', 'Excl FIFO get times', 'Block implementation call', 'FIFO return buffer times']
  plotted_atomids = []

  #plot stats
  horz = True

  btfname = tfname

  for ii in [0,1,2,3,4]:

    stats = atomi_stats_a[ii]

    atomids = [atomid for (atomid, stat_tup) in stats]
    stat_tups = [stat_tup for (atomid, stat_tup) in stats]
    means   = [mean   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    medians = [median for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    mins    = [minn   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    maxs    = [maxx   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    pctls95 = [pctl95 for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]

    tfname = btfname.split('.png')[0] + "subplot_" + str(ii) + '.png'
    plt.figure(figsize=(8,8))
    #plt.subplot(2,5,ii+1)

    plotted_atomids = atomids


    #plt.margins(0.1)	#extend axis limits by this fraction
    plt.margins(0.05)	#extend axis limits by this fraction
    plt.title(titles[ii])

    ids = range(len(atomids))
    if (horz):
      plt.plot(ids, means   ,  'b.-')
      plt.plot(ids, maxs    ,  'r.-')
      plt.plot(ids, mins    ,  'g.-')
      plt.plot(ids, medians ,  'k.--')
      plt.plot(ids, pctls95 ,  'k.-')

      plt.xlabel('Atom ID')
      plt.ylabel('Cycles')
    else:
      plt.plot(means   ,ids,  'b.-')
      plt.plot(maxs    ,ids,  'r.-')
      plt.plot(mins    ,ids,  'g.-')
      plt.plot(medians ,ids,  'k.--')
      plt.plot(pctls95 ,ids,  'k.-')

      plt.ylabel('Atom ID')
      plt.xlabel('Cycles')
    plt.grid(True)

    plt.savefig(tfname)


#  ax = plt.subplot(2,5,1)
#  ys = ids
#  ats = plotted_atomids
#
#  if not horz:
#    for y, at in zip(ys, ats):
#      ax.annotate(atomi_labels[at], xy=(0,y), rotation=0, fontsize='x-small')
#
#  ax = plt.subplot(2,5,5)
#  ax.legend(legendvals, loc='center left', bbox_to_anchor=(1, 0.5))

  #plot timeseries
  atomi_durs_fg, atomi_durs_ii, atomi_durs_fd, atomi_durs_fw, atomi_durs_fgmw  = atomi_durs 
  atomi_durs_a = [atomi_durs_fg, atomi_durs_fw, atomi_durs_fgmw, atomi_durs_ii, atomi_durs_fd] 
  for ii in [0,1,2,3,4]:
    labels = []
    durs = atomi_durs_a[ii]
    print durs
    #durs_fg[idv1] = [t_fg]

    #plt.subplot(2,5,ii+6)
    tfname = btfname.split('.png')[0] + "subplot_" + str(ii + 5) + '.png'

    plt.figure(figsize=(8,8))

    for atomid, durations in durs:
      plt.plot(durations[0:200], '.-')
      labels.append(atomi_labels[atomid])

    plt.ylabel('Cycles')
    plt.xlabel('Invocation number')
    plt.grid(True)
    plt.title('Timeseries')

    plt.savefig(tfname)


  #ax = plt.subplot(2,5,10)
  #ax.legend(labels, loc='center left', bbox_to_anchor=(1, 0.5), fontsize='x-small')

  #plt.suptitle('Atom internals and FIFO module costs')

  #plt.show()
  #plt.savefig(tfname)



def plot_atomis_i_one_figure(tfname, atomi_stats, atomi_labels, atomi_durs):
  atomi_stats_fg, atomi_stats_ii, atomi_stats_fd, atomi_stats_fw, atomi_stats_fgmw  = atomi_stats 
  atomi_stats_a = [atomi_stats_fg, atomi_stats_fw, atomi_stats_fgmw, atomi_stats_ii, atomi_stats_fd] 

  #plt.figure(figsize=(20,10))
  plt.figure(figsize=(50,20))

  legendvals = ['mean', 'max', 'min', 'median','95%tile']
  titles = ['FIFO get buffer times', 'FIFO_wait_times', 'Excl FIFO get times', 'Block implementation call', 'FIFO return buffer times']
  plotted_atomids = []

  #plot stats
  horz = True

  for ii in [0,1,2,3,4]:
    stats = atomi_stats_a[ii]

    atomids = [atomid for (atomid, stat_tup) in stats]
    stat_tups = [stat_tup for (atomid, stat_tup) in stats]
    means   = [mean   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    medians = [median for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    mins    = [minn   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    maxs    = [maxx   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
    pctls95 = [pctl95 for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]

    plt.subplot(2,5,ii+1)

    plotted_atomids = atomids


    #plt.margins(0.1)	#extend axis limits by this fraction
    plt.margins(0.05)	#extend axis limits by this fraction
    plt.title(titles[ii])

    ids = range(len(atomids))
    if (horz):
      plt.plot(ids, means   ,  'b.-')
      plt.plot(ids, maxs    ,  'r.-')
      plt.plot(ids, mins    ,  'g.-')
      plt.plot(ids, medians ,  'k.--')
      plt.plot(ids, pctls95 ,  'k.-')

      plt.xlabel('Atom ID')
      plt.ylabel('Cycles')
    else:
      plt.plot(means   ,ids,  'b.-')
      plt.plot(maxs    ,ids,  'r.-')
      plt.plot(mins    ,ids,  'g.-')
      plt.plot(medians ,ids,  'k.--')
      plt.plot(pctls95 ,ids,  'k.-')

      plt.ylabel('Atom ID')
      plt.xlabel('Cycles')
    plt.grid(True)


  ax = plt.subplot(2,5,1)
  ys = ids
  ats = plotted_atomids

  if not horz:
    for y, at in zip(ys, ats):
      ax.annotate(atomi_labels[at], xy=(0,y), rotation=0, fontsize='x-small')

  ax = plt.subplot(2,5,5)
  ax.legend(legendvals, loc='center left', bbox_to_anchor=(1, 0.5))

  #plot timeseries
  atomi_durs_fg, atomi_durs_ii, atomi_durs_fd, atomi_durs_fw, atomi_durs_fgmw  = atomi_durs 
  atomi_durs_a = [atomi_durs_fg, atomi_durs_fw, atomi_durs_fgmw, atomi_durs_ii, atomi_durs_fd] 
  for ii in [0,1,2,3,4]:
    labels = []
    durs = atomi_durs_a[ii]
    print durs
    #durs_fg[idv1] = [t_fg]

    plt.subplot(2,5,ii+6)
    for atomid, durations in durs:
      plt.plot(durations[0:200], '.-')
      labels.append(atomi_labels[atomid])

    plt.ylabel('Cycles')
    plt.xlabel('Invocation number')
    plt.grid(True)
    plt.title('Timeseries')

  ax = plt.subplot(2,5,10)
  ax.legend(labels, loc='center left', bbox_to_anchor=(1, 0.5), fontsize='x-small')

  plt.suptitle('Atom internals and FIFO module costs')

  #plt.show()
  plt.savefig(tfname)



def plot_atoms(tf, atom_stats_sorted, atom_labels, atom_stats_norm, conf={}):
  tfname =  '%s_plot_atoms.png' % tf
  plot_atoms_i(tfname, atom_stats_sorted, atom_labels)
  tfname =  '%s_plot_atoms_norm.png' % tf
  plot_atoms_i(tfname, atom_stats_norm, atom_labels)

  tfname =  '%s_plot_atoms_horz.png' % tf
  tfname_labelmap =  '%s_plot_atoms_horz_labelmap.txt' % tf
  plot_atoms_i_horz(tfname, atom_stats_sorted, atom_labels, tfname_labelmap)
  tfname =  '%s_plot_atoms_norm_horz.png' % tf
  tfname_labelmap =  '%s_plot_atoms_norm_horz_labelmap.txt' % tf
  plot_atoms_i_horz(tfname, atom_stats_norm, atom_labels, tfname_labelmap)

def plot_atomis(tf, atomi_stats, atomi_labels, atomi_durs):
  tfname =  '%s_plot_atomis.png' % tf
  plot_atomis_i(tfname, atomi_stats, atomi_labels, atomi_durs)


def plot_atom_timeseries(tf, atom_stats_norm, all_durs, atom_labels_dict, more_atomids=[], conf={}):
  tfname =  '%s_plot_atoms_timeseries.png' % tf
  worst_how_many = 3
  #fs = ['r.-','g.-','b.-']

  #re-sort by normalized max
  atom_stats_norm_resorted = sorted(atom_stats_norm, key=lambda st: st[1][4], reverse=True)

  worst_top = []
  worst_top_labels = []
  for atom_stat in atom_stats_norm_resorted[0:worst_how_many]:
    atom_id, (cnt, mean, std, minn, maxx, diff, median, pctl95) = atom_stat
    worst_top.append(atom_id)
    worst_top_labels.append(atom_labels_dict[atom_id])
    print atom_labels_dict[atom_id]

  print more_atomids
  if more_atomids:
    for atom_stat in atom_stats_norm_resorted:
      atom_id, (cnt, mean, std, minn, maxx, diff, median, pctl95) = atom_stat
      if atom_id in more_atomids:
        print 'adding atomid to timeseries', atom_id
	worst_top.append(atom_id)
	worst_top_labels.append(atom_labels_dict[atom_id])
	print atom_labels_dict[atom_id]

  plt.figure()
  plt.figure(figsize=(160,8))
  plt.margins(0.1)	#extend axis limits by this fraction
  plt.subplots_adjust(left=0.15, right=0.9, bottom=0.2, top=0.85)
  plt.title('Atom compute times timeseries\n%s' % os.getcwd().split('orsys')[1])
  #for atom_id, f in zip(worst_top, fs):
  for atom_id in worst_top:
    #plt.plot(all_durs[atom_id], f)
    plt.plot(all_durs[atom_id])
    #plt.plot(range(1,1+len(all_durs[atom_id])), [int(t) for t in all_durs[atom_id]], f)
    #plt.plot(range(1,1+len(all_durs[atom_id])), [int(t) for t in all_durs[atom_id]], '.-')

  plt.ylabel('Cycles')
  plt.xlabel('Call number')
  plt.grid(True)


  #------
  ax = plt.subplot(111)
  # Shink current axis by 20%
  #box = ax.get_position()
  #ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])

  # Put a legend to the right of the current axis
  #plt.legend(worst_top_labels, loc='upper right')
  #ax.legend(worst_top_labels, loc='center left', bbox_to_anchor=(1, 0.5))


  #------
  # Shink current axis's height by 10% on the bottom
  box = ax.get_position()
  ax.set_position([box.x0, box.y0 + box.height * 0.2,
		   box.width, box.height * 0.8])
  # Put a legend below current axis
  #ax.legend(worst_top_labels, loc='upper center', bbox_to_anchor=(0.5, -0.05))#,
  ax.legend(worst_top_labels, loc='upper center', bbox_to_anchor=(0.5, -0.13))#,
           # fancybox=True, shadow=True, ncol=5)

  #plt.show()
  plt.savefig(tfname)

def plot_atom_tseqs(tf, atid_tseqs, atom_labels_dict):
  tfname = '%s_plot_atoms_tseqs.png' % (tf)
  plt.figure()
  plt.figure(figsize=(30,30))
  plt.title('AtomId time sequences\n%s' % (os.getcwd().split('orsys')[1]))
  all_xs = []
  all_ys = []
  dy = 0.2
  #f=['r.-','g.-','b.-','k.-']
  f=['r-','g-','b-','k-']
  for coreid, atid_tseq in enumerate(atid_tseqs):
    xs = []
    ys = []
    for (atid, (ts1, ts2)) in atid_tseq:
      #ys.append(0)
      ys.append(atid + coreid * dy)
      ys.append(atid + coreid * dy)
      #ys.append(0)
      #xs.append(ts1)
      xs.append(ts1)
      xs.append(ts2)
      #xs.append(ts2)
      y = atid + coreid * dy
      plt.plot([ts1, ts2], [y,y], f[coreid])

    all_xs.append(xs)
    all_ys.append(ys)

  #plt.plot(all_xs[0], all_ys[0], 'r.-', linewidth=3)
  #plt.plot(all_xs[1], all_ys[1], 'g.-', linewidth=3)
  #plt.plot(all_xs[2], all_ys[2], 'b.-', linewidth=3)
  #plt.plot(all_xs[3], all_ys[3], 'k.-', linewidth=3)

  plt.ylabel('AtomId')
  plt.xlabel('Cycle')
  plt.grid(True)

  plt.savefig(tfname)

  #----------------------------------------------
  f=['r.-','g.-','b.-','k.-']
  for coreid, atid_tseq in enumerate(atid_tseqs):
    tfname = '%s_plot_atoms_tseqs_%d.png' % (tf, coreid)
    plt.figure()
    plt.figure(figsize=(30,30))
    plt.title('AtomId time sequence, core %d\n%s' % (coreid,os.getcwd().split('orsys')[1]))
    xs = []
    ys = []
    for (atid, (ts1, ts2)) in atid_tseq:
      xs = []
      ys = []
      ys.append(0)
      ys.append(atid + coreid * dy)
      ys.append(atid + coreid * dy)
      ys.append(0)
      xs.append(ts1)
      xs.append(ts1)
      xs.append(ts2)
      xs.append(ts2)
      y = atid + coreid * dy
      plt.plot(xs, ys, f[coreid])
    plt.ylabel('AtomId')
    plt.xlabel('Cycle')
    plt.grid(True)

    plt.savefig(tfname)
  #----------------------------------------------
  fig, axs = plt.subplots(4, sharex=True, sharey=True, figsize=(30,30))
  #plt.figure(figsize=(30,30))
  f=['r.-','g.-','b.-','k.-']
  #pltids=[411,412,413,414]
  tfname = '%s_plot_atoms_tseqs_all.png' % (tf)
  #f, (ax1, ax2, ax3, ax4) = plt.subplots(4, sharex=True, sharey=True)
  #ax1.plot(x, y)
  #ax1.set_title('Sharing both axes')
  #ax2.scatter(x, y)
  #ax3.scatter(x, 2 * y ** 2 - 1, color='r')
  # Fine-tune figure; make subplots close to each other and hide x ticks for
  # all but bottom plot.
  for coreid, atid_tseq in enumerate(atid_tseqs):
    #ax = plt.subplot(pltids[coreid])
    xs = []
    ys = []
    for (atid, (ts1, ts2)) in atid_tseq:
      xs = []
      ys = []
      ys.append(0)
      ys.append(atid + coreid * dy)
      ys.append(atid + coreid * dy)
      ys.append(0)
      xs.append(ts1)
      xs.append(ts1)
      xs.append(ts2)
      xs.append(ts2)
      y = atid + coreid * dy
      #ax.plot(xs, ys, f[coreid])
      #ax.grid(True)
      axs[coreid].plot(xs, ys, f[coreid])
      axs[coreid].grid(True)
  plt.ylabel('AtomId')
  plt.xlabel('Cycle')
  #plt.title('AtomId time sequence\n%s' % (os.getcwd().split('orsys')[1]))
  axs[0].set_title('AtomId time sequence\n%s' % (os.getcwd().split('orsys')[1]))

  fig.subplots_adjust(hspace=0)
  #plt.setp([a.get_xticklabels() for a in fig.axes[:-1]], visible=False)

  plt.savefig(tfname)



#------------------------------------------------------------
