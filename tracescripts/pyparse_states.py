'''
Atomix project, pyparse_states.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import numpy as np
import pickle
import os
import sys
import math
from pyparse_util import sorted_interval_match

def state_labels_from_file(state_profiling_table_file):
  #------------------------------------------------------------------
  stlabels = {}
  stidfromlabel = {}
  f = open(state_profiling_table_file, 'r')
  stpt = [l.strip().split(',') for l in f.readlines()]
  for l in stpt:
    stlabels[l[0]] = l[1]
  f.close()
  #print stpt
  #print stlabels
  #------------------------------------------------------------------
  for key in stlabels:
    stidfromlabel[stlabels[key]] = key
  return stlabels, stidfromlabel


def state_synchronize_timestamps(ddtup, syncState):
  #------------------------------------------------------------------
  ## synchronize timestamps from different cores ##
  (dd0, dd1, dd2, dd3) = ddtup

  for idv,ts in dd0:
    if idv == syncState:
      break
  t0ss = ts

  for idv,ts in dd1:
    if idv == syncState:
      break
  t1ss = ts

  for idv,ts in dd2:
    if idv == syncState:
      break
  t2ss = ts

  for idv,ts in dd3:
    if idv == syncState:
      break
  t3ss = ts

  print 'syncState entry times: ', t0ss, t1ss, t2ss, t3ss
  off1 = t1ss - t0ss
  off2 = t2ss - t0ss
  off3 = t3ss - t0ss
  print 'Offsets: ', off1, off2, off3

  dd1 = [(idv,ts - off1) for (idv,ts) in dd1]
  dd2 = [(idv,ts - off2) for (idv,ts) in dd2]
  dd3 = [(idv,ts - off3) for (idv,ts) in dd3]

  tmin0 = dd0[0][1]
  tmin1 = dd1[0][1]
  tmin2 = dd2[0][1]
  tmin3 = dd3[0][1]

  tmin = min([tmin0, tmin1, tmin2, tmin3])
  print 'tmin: ', tmin

  dd0 = [(idv,int((ts)-tmin)) for (idv,ts) in dd0]
  dd1 = [(idv,int((ts)-tmin)) for (idv,ts) in dd1]
  dd2 = [(idv,int((ts)-tmin)) for (idv,ts) in dd2]
  dd3 = [(idv,int((ts)-tmin)) for (idv,ts) in dd3]
  #------------------------------------------------------------------
  ddtup = (dd0, dd1, dd2, dd3)
  return ddtup

def state_separate_out_timestamps(ddtup, fifo_wait_ts_ids, fifo_wait_ts_ids_to_plot, trace_reader_ts_ids):
  (dd0, dd1, dd2, dd3) = ddtup
  #------------------------------------------------------------------
  fmin, fmax = fifo_wait_ts_ids
  fmin_to_plot, fmax_to_plot = fifo_wait_ts_ids_to_plot
  trin, trout = trace_reader_ts_ids

  #separate out fifo wait timestamps
  dd0_w = [(idv,ts) for (idv,ts) in dd0 if int(idv) >= fmin_to_plot and int(idv) <= fmax_to_plot]
  dd1_w = [(idv,ts) for (idv,ts) in dd1 if int(idv) >= fmin_to_plot and int(idv) <= fmax_to_plot]
  dd2_w = [(idv,ts) for (idv,ts) in dd2 if int(idv) >= fmin_to_plot and int(idv) <= fmax_to_plot]
  dd3_w = [(idv,ts) for (idv,ts) in dd3 if int(idv) >= fmin_to_plot and int(idv) <= fmax_to_plot]

  #separate out tracereader timestamps
  dd0_tr = [(idv,ts) for (idv,ts) in dd0 if int(idv) >= trin and int(idv) <= trout]
  dd1_tr = [(idv,ts) for (idv,ts) in dd1 if int(idv) >= trin and int(idv) <= trout]
  dd2_tr = [(idv,ts) for (idv,ts) in dd2 if int(idv) >= trin and int(idv) <= trout]
  dd3_tr = [(idv,ts) for (idv,ts) in dd3 if int(idv) >= trin and int(idv) <= trout]

  #keep only state timestamps
  dd0 = [(idv,ts) for (idv,ts) in dd0 if int(idv) < fmin] 
  dd1 = [(idv,ts) for (idv,ts) in dd1 if int(idv) < fmin] 
  dd2 = [(idv,ts) for (idv,ts) in dd2 if int(idv) < fmin] 
  dd3 = [(idv,ts) for (idv,ts) in dd3 if int(idv) < fmin] 
  #------------------------------------------------------------------
  ddtup = (dd0, dd1, dd2, dd3)
  ddtup_w = (dd0_w, dd1_w, dd2_w, dd3_w)
  ddtup_tr = (dd0_tr, dd1_tr, dd2_tr, dd3_tr)
  return ddtup, ddtup_w, ddtup_tr

def state_dump_data(coreid_s_tin_tout_pktn_repn, stlabels, \
	coreid_wcyin_wcyout, coreid_bcyin_bcyout, core_utils):

  #------------------------------------------------------------------
  ## dump processed data 
  outfname = '__data_state_transitions.txt'
  f = open(outfname, 'w')

  for (coreid,s,tin,tout,pktn,repn) in coreid_s_tin_tout_pktn_repn:
    lb = stlabels[s]
    print >>f, "coreid=%s,st=%s,cyin=%s,cyout=%s,pktn=%s,repn=%s,lb=%s" % \
    	(coreid, s, tin, tout, pktn, repn, lb)

  print 'saved pkt-labeled state transitions to', outfname

  #------------------------------------------------------------------


  outfname = '__data_fifo_waits.txt'
  f = open(outfname, 'w')
  for coreid,cyin,cyout in coreid_wcyin_wcyout:
    print >>f, "coreid=%s,cyin_wait=%s,cyout_wait=%s" % (coreid,cyin,cyout)
  f.close()

  #------------------------------------------------------------------

  outfname = '__data_busy_map.txt'
  f = open(outfname, 'w')
  for coreid,cyin,cyout in coreid_bcyin_bcyout:
    print >>f, "coreid=%s,cyin_busy=%s,cyout_busy=%s" % (coreid,cyin,cyout)
  f.close()

  #------------------------------------------------------------------

  outfname = '__data_busy_map.txt'
  f = open(outfname, 'w')
  for coreid,cyin,cyout in coreid_bcyin_bcyout:
    print >>f, "coreid=%s,cyin_busy=%s,cyout_busy=%s" % (coreid,cyin,cyout)
  f.close()

  #------------------------------------------------------------------

  outfname = '__data_core_util_stats.txt'
  f = open(outfname, 'w')
  for coreid,util_pct in enumerate(core_utils):
    print >>f, "coreid=%s,util_pct=%3.1f" % (coreid,util_pct)
  f.close()


def compute_core_util_stats(coreid_bcyin_bcyout, coreid_s_tin_tout_pktn_repn):
  a1 = coreid_bcyin_bcyout
  a2 = coreid_s_tin_tout_pktn_repn
  ts = [(int(coreid),int(cyin),int(cyout),int(pktn)) for coreid,st,cyin,cyout,pktn,repn in a2]
  n_pkts = max([pktn for _,_,_,pktn in ts]) + 1
  n_cores = max([coreid for coreid,_,_,_ in ts]) + 1
  tot_durs = []
  core_durs = [[]] * n_cores
  for pktn in range(n_pkts):
    tsn = [t for t in ts if t[3] == pktn]
    t_pkt_in = float(tsn[0][1])
    t_pkt_out = float(tsn[-1][2])
    tot_durs.append(t_pkt_out - t_pkt_in)
    for coreid in range(n_cores):
      tsc = [t for t in a1 if int(t[0]) == coreid]
      durs = [max(min(t_pkt_out,int(t[2])) - max(t_pkt_in,int(t[1])), 0) for t in tsc]
      a = core_durs[coreid] 
      a = a + durs
      core_durs[coreid] = a
  #print tot_durs
  #print core_durs
  #raw_input()
  core_durs = [float(sum(a)) for a in core_durs]
  tot_dur = sum(tot_durs)
  b = tot_dur
  core_utils = [100*a/b for a in core_durs]
  #print core_utils
  #raw_input()
  return core_utils

def state_extract_wait_and_busy_data(ddtup_w):
  (dd0, dd1, dd2, dd3) = ddtup_w
  coreid = 0
  coreid_wcyin_wcyout = []
  for dd in ddtup_w:
    win_wout = zip(dd[0::2],dd[1::2])
    for pair in win_wout:
      (_,cyin), (_,cyout) = pair
      coreid_wcyin_wcyout.append((coreid,cyin,cyout))
    coreid += 1

  (dd0, dd1, dd2, dd3) = ddtup_w
  coreid = 0
  coreid_bcyin_bcyout = []
  for dd in ddtup_w:
    cyin_busy = [('0',0)] + dd[1::2]
    cyin_busy = cyin_busy[:-1]
    cyout_busy = dd[0::2]
    for pair in zip(cyin_busy, cyout_busy):
      (_,cyin_busy), (_,cyout_busy) = pair
      coreid_bcyin_bcyout.append((coreid,cyin_busy,cyout_busy))
    coreid += 1

  return coreid_wcyin_wcyout, coreid_bcyin_bcyout

def state_extract_packet_windows_arbitrary_states(ddtup, stin, stout, stidfromlabel):
  stlbin,coreidin = tuple(stin.split(':'))
  stlbout,coreidout = tuple(stout.split(':'))
  coreidin = int(coreidin); coreidout = int(coreidout)
  stin = stidfromlabel[stlbin]
  stout = stidfromlabel[stlbout]
  #print stlbin, stin, stlbout, stout, coreidin, coreidout
  (dd0, dd1, dd2, dd3) = ddtup
  pktn_en_ex_ix_tups = []
  for dd in ddtup:
    in_idxs = [i for i,(s,t) in enumerate(dd) if s == stin]
    in_idxs = in_idxs[::2]
    out_idxs = [i for i,(s,t) in enumerate(dd) if s == stout]
    out_idxs = out_idxs[1::2]
    pktn_entry_exit_idx_tuples = [e for e in enumerate(zip(in_idxs,out_idxs))]
    #print pktn_entry_exit_idx_tuples
    pktn_en_ex_ix_tups.append(pktn_entry_exit_idx_tuples)

  #print pktn_en_ex_ix_tups
  #array of array of tuples. top-level array contains an array for each core. each core 
  #array contains tuples. each tuple indicates a pkt with the following structure:
  #(pktid, (entry_idx, exit_idx))
  #where the idx is on dd array for that core.

  #sample and hold pktid information
  pktid_expanded_lists = []
  for l in pktn_en_ex_ix_tups:
    pktid_expanded_list = []
    prev_exit_idx = -1
    for pktid, (entry_idx, exit_idx) in l:
      #print pktid, entry_idx, exit_idx
      a1 = [-1] * (entry_idx - prev_exit_idx - 1)
      a2 = [pktid] * (exit_idx - entry_idx + 1)
      a = a1 + a2
      pktid_expanded_list += a
      prev_exit_idx = exit_idx
    #print pktid_expanded_list
    pktid_expanded_lists.append(pktid_expanded_list)

  #attach pid column
  dd_pids = []
  for dd, pidlist in zip(ddtup, pktid_expanded_lists):
  #(for each core)
    idx_dd = [e for e in enumerate(dd)]
    dd_pid = zip(dd, pidlist)

    #double up states so that entry and exit are on the same row
    dd_pid_in = dd_pid[::2]
    dd_pid_out = dd_pid[1::2]
    dd_pid = zip(dd_pid_in,dd_pid_out)


    dd_pids.append(dd_pid)
    #schema: (s,t),pktnumber,(s,t),pktnumber	
    #(pktnumber is -1 for parts that are not in pkt states)
    #first (s,t) tuple indicates state entry, second indicates state exit

    #for e in dd_pid:
    #  print e

  #deduce state repeat counts
  dd_pid_rep_coreid = []
  for coreid, dd_pid in enumerate(dd_pids):
    ppid = -1
    ps = '-1'
    repcount = 0
    repcounts = []
    for ((s,t),pid),((sout,tout),pid) in dd_pid:
      if s != sout:
        print 'ERROR: state id mismatch on the same row'
	sys.exit(1)
      if s == ps:  #repeat state
        if pid == ppid: #same pkt
	  repcount += 1
	else:
	  repcount = 0
      else:
	repcount = 0
      repcounts.append(repcount)
      ps = s
      ppid = pid
    #attach coreids too
    coreidexp = [coreid] * len(dd_pid)
    dd_pid_rep_cid = zip(dd_pid,repcounts,coreidexp)
    dd_pid_rep_coreid += dd_pid_rep_cid

  print '----------------'
  #clean up redundant columns
  coreid_s_tin_tout_pktn_repn = [(coreid,s,tin,tout,pktn,repn) for \
  	((((s,tin),pktn),((_,tout),_)),repn,coreid) in dd_pid_rep_coreid]
  #for e in coreid_s_tin_tout_pktn_repn:
  #  print e

  return coreid_s_tin_tout_pktn_repn  



def state_extract_packet_windows(ddtup, stin, stout):
  (dd0, dd1, dd2, dd3) = ddtup
  #------------------------------------------------------------------
  ## extract data-decode windows based on entry and exit states ##
  win_on = 0
  win = []
  allwins = []
  tin = []
  tout = []
  for r in zip(dd0, dd1, dd2, dd3):
    d0, d1, d2, d3 = r
    s0,t0 = d0
    s1,t1 = d1
    s2,t2 = d2
    s3,t3 = d3
    if (len(set([s0,s1,s2,s3])) > 1):
      print 'States do not match'
      print r
      sys.exit(1)

    if (s0 == stin and win_on == 0):
      win_on = 1
      tin.append([t0, t1, t2, t3])

    if win_on:
      win.append(r)

    if (s0 == stout and win_on == 1):
      win_on = 0
      tout.append([t0, t1, t2, t3])
      allwins.append(win)
      win = []

  print len(allwins), 'data-decode windows extracted'
  print allwins
  #------------------------------------------------------------------
  return allwins, tin, tout

def state_completion_time_stats(allwins, tin, tout):
  #------------------------------------------------------------------
  ## completion times ##
  #print zip(tin, tout)
  completion_times = []
  for i, ti, to, win in zip(range(len(allwins)), tin, tout, allwins):
    print 'Display window #', i
    tmin = min(min(ti), min(to))
    print 'tmin=',tmin
    ti = [t - tmin for t in ti]
    to = [t - tmin for t in to]
    print ti, to

    completion_times.append(max(to))
    
    dd0 = [d0 for d0, d1, d2, d3 in win]
    dd1 = [d1 for d0, d1, d2, d3 in win]
    dd2 = [d2 for d0, d1, d2, d3 in win]
    dd3 = [d3 for d0, d1, d2, d3 in win]

    dd0 = [(idv,int((ts)-tmin)) for (idv,ts) in dd0]
    dd1 = [(idv,int((ts)-tmin)) for (idv,ts) in dd1]
    dd2 = [(idv,int((ts)-tmin)) for (idv,ts) in dd2]
    dd3 = [(idv,int((ts)-tmin)) for (idv,ts) in dd3]

    for r in zip(dd0, dd1, dd2, dd3):
      print r
  #------------------------------------------------------------------


  #------------------------------------------------------------------
  completion_time_mean = np.mean(completion_times)
  completion_time_std = np.std(completion_times)
  completion_time_max = max(completion_times)
  completion_time_min = min(completion_times)
  completion_time_diff = completion_time_max - completion_time_min
  completion_time_mean = np.mean(completion_times)

  y = completion_time_mean
  ys = completion_times
  erry = completion_time_diff
  #------------------------------------------------------------------
  state_stats = (y, erry, ys)
  last_tmin = tmin
  return state_stats, last_tmin

def state_pickledepickle_stat_data(state_stats):
  #------------------------------------------------------------------
  y, erry, ys = state_stats
  pltData = [y, erry, ys]
  print os.getcwd()
  tag = os.getcwd().split('trace')[1].split('/')[-1]
  print tag
  fname = '../st_hist_stats.pkl'
  try:
    f = open(fname,'rb')
    try:
      d = pickle.load(f)
    except EOFError:
      d = {}
    f.close()
  except IOError:
    d = {}

  f = open(fname, 'wb')
  d[tag] = pltData
  pickle.dump(d, f)
  f.close()
  print 'Saved st plot data to %s' % fname
  #------------------------------------------------------------------

def state_proc_time_cdf(state_stats):
  y, erry, ys = state_stats
  completion_times = ys
  #------------------------------------------------------------------
  hist, bin_edges = np.histogram(completion_times, 20, density=True)
  print bin_edges
  print hist
  hist = hist * np.diff(bin_edges)
  csum = np.cumsum(hist) 
  #------------------------------------------------------------------
  state_stats = (y, erry, ys, csum, bin_edges)
  return state_stats


def state_waitwins_getlastwin(ddtup, ddtup_w, last_tmin):
  (dd0,dd1,dd2,dd3) = ddtup
  (dd0_w,dd1_w,dd2_w,dd3_w) = ddtup_w
  print len(dd0), len(dd1), len(dd2), len(dd3)

  tmin = last_tmin

  #------------------------------------------------------------------
  #normalized ts's to the start time of the last window 
  #and filter to keep only the waits of the last window
  tmax = dd0[-2][1]
  dd0_w = [(idv,ts-tmin) for (idv,ts) in dd0_w if ts >= tmin and ts - tmin <= tmax] 
  tmax = dd1[-2][1]
  dd1_w = [(idv,ts-tmin) for (idv,ts) in dd1_w if ts >= tmin and ts - tmin <= tmax] 
  tmax = dd2[-2][1]
  dd2_w = [(idv,ts-tmin) for (idv,ts) in dd2_w if ts >= tmin and ts - tmin <= tmax] 
  tmax = dd3[-2][1]
  dd3_w = [(idv,ts-tmin) for (idv,ts) in dd3_w if ts >= tmin and ts - tmin <= tmax] 

  ddws = [dd0_w,dd1_w,dd2_w,dd3_w]
  dds = [dd0,dd1,dd2,dd3]

  return (ddws, dds)

def state_waitwins(ddtup, ddtup_w, lastwin_only=False, last_tmin=0):
  if lastwin_only:
    (dd0,dd1,dd2,dd3) = ddtup
    (dd0_w,dd1_w,dd2_w,dd3_w) = ddtup_w
    ddws, dds = state_waitwins_getlastwin(ddtup, ddtup_w, last_tmin)
  else:
    (dd0,dd1,dd2,dd3) = ddtup
    (dd0_w,dd1_w,dd2_w,dd3_w) = ddtup_w
    print len(dd0), len(dd1), len(dd2), len(dd3)

    ddws = [dd0_w,dd1_w,dd2_w,dd3_w]
    dds = [dd0,dd1,dd2,dd3]

  coreid = 0
  waitwins = []
  print 'dd0_top:', dd0
  for ddw,dd in zip(ddws,dds):
    ts_in  = [(int(id), ts) for (id, ts) in ddw[::2]]
    ts_out = [(int(id), ts) for (id, ts) in ddw[1::2]]
    wins = zip(ts_in, ts_out)

    #wins_st = []
    #idx = 1
    #state_exit = dd[idx]
    #state_id, state_exit_ts = state_exit
    #for win in wins:
    #  win_end = win[1][1]
    #  if win_end > state_exit_ts:
    #    idx += 2
    #    print 'coreid=',coreid,'idx=',idx,'win_end=',win_end
    #    state_exit = dd[idx]
    #    state_id = state_exit[0]
    #    state_exit_ts = state_exit[1]
    #    if win_end > state_exit_ts:
    #      print "unexpected error: win_end: ", win_end, " state_exit_ts:", state_exit_ts
    #      sys.exit(1)
    #  wins_st.append((state_id, coreid))
    #print wins_st
    #wins = zip(wins_st, wins)
    xxc = wins
    bk = dd; bkin = bk[::2]; bkout = bk[1::2]; bkc = zip(bkin, bkout)
    wins = sorted_interval_match(xxc, bkc)
    wins = [((st,coreid),a) for (a,((st,_),(_,_))) in wins]
    print wins
    #import pdb; pdb.set_trace()

      
    #validate wins
    nmismatches = sum([idout - idin - 1 for _,((idin, _), (idout, _)) in wins])
    if nmismatches > 0: 
      print 'wait ts ids on coreid', coreid, 'have', nmismatches, 'mismatches'
      sys.exit(1)

    waitwins.append(wins)
    #for win in wins:
    #  print win

    coreid += 1
  #------------------------------------------------------------------

  return waitwins


def analyze_tracereads(ddtup_tr, coreid_s_tin_tout_pktn_repn, trace_reader_ts_ids):
  #print 'analyze_tracereads'
  tri, tro = trace_reader_ts_ids 
  tri = str(tri)
  tro = str(tro)
  l0 = [(coreid,s,tin,tout,pktn,repn) for (coreid,s,tin,tout,pktn,repn) \
  	in coreid_s_tin_tout_pktn_repn if coreid == 0]# and pktn >= 0]
  #for e in l0:
  #  print e

  #join table and trace reads
  dd0,_,_,_ = ddtup_tr
  #print dd0
  ii = iter(l0)
  e = next(ii)
  coreid,s,tin,tout,pktn,repn = e
  trs = []
  for f in dd0:
    lb, ts = f
    while ts > tin:
      try:
	e = next(ii)
	coreid,s,tin,tout,pktn,repn = e
      except StopIteration:
        break
    if ts > tout:
      print 'warning: no containing state interval for trace reader entry %s' % str(f)
      trs.append((f, (-2, -2, -2, -2, -2, -2)))
    else:
      trs.append((f, e))
    #print trs[-1]

  trins = [(lb, ts, pktn) for ((lb, ts), (_,_,_,_,pktn,_)) in trs if lb == tri]
  #print trins

  counts = []
  e = trins[0]
  lb, ts, pktn = e
  count = (pktn, 0)
  for e in trins:
    nlb, nts, npktn = e
    if npktn == pktn:
      pktn, c = count
      c += 1
      count = (pktn, c)
    else:
      counts.append(count)
      pktn = npktn
      count = (pktn, 1)
  _, c = count
  if c > 0:
    counts.append(count)
  #print counts

  trcounts = counts

  #print 'trcounts = ', trcounts
  #trcounts will show up as [(pktn,count)] where pktn will go
  #as -1, 0, -1, 1, ..., corresponding roughly to pad, real-pkt, pad, real-pkt...
  #Counts that belong to pktn >= 0 correspond to trace reads that happened within
  #a packet's receive state endpoints. However, the exact pkt start point may not
  #necessarily lie at the pad/pkt read boundary.
  padreads = trcounts[::2]
  pktreads = trcounts[1::2]
  #print padreads
  #print pktreads
  totreads = zip(padreads, pktreads)
  #print totreads
  totreads = [(pktn, c1+c2) for (_,c1),(pktn,c2) in totreads]
  #print totreads
  totrcnts = [c for _,c in totreads]
  cumrcnts = list(np.cumsum(totrcnts))
  #print totrcnts
  #print cumrcnts

  return trins, trcounts, totrcnts, cumrcnts

def parse_states(cmd, tf, ddtup, state_profiling_table_file, conf):
  stin 			= conf['stin']
  stout 		= conf['stout']
  syncState 		= conf['syncState']
  fifo_wait_ts_ids 	= conf['fifo_wait_ts_ids']
  fifo_wait_ts_ids_to_plot = conf['fifo_wait_ts_ids_to_plot']
  tri 			= conf['traceReaderInTSID']
  tro 			= conf['traceReaderOutTSID']

  fmin, fmax = fifo_wait_ts_ids
  fifo_wait_ts_ids = (int(fmin), int(fmax))
  fmin_to_plot, fmax_to_plot = fifo_wait_ts_ids
  fifo_wait_ts_ids_to_plot = (int(fmin_to_plot), int(fmax_to_plot))
  trace_reader_ts_ids = (int(tri), int(tro))

  stlabels, stidfromlabel = state_labels_from_file(state_profiling_table_file)
  syncState = [key for key in stlabels if stlabels[key] == syncState]

  ddtup = state_synchronize_timestamps(ddtup, syncState)

  ddtup, ddtup_w, ddtup_tr = \
  	state_separate_out_timestamps(ddtup, fifo_wait_ts_ids, fifo_wait_ts_ids_to_plot, trace_reader_ts_ids)

  coreid_s_tin_tout_pktn_repn = state_extract_packet_windows_arbitrary_states(\
  	ddtup, stin, stout, stidfromlabel)

  coreid_wcyin_wcyout, coreid_bcyin_bcyout = state_extract_wait_and_busy_data(\
  	ddtup_w)

  core_utils = compute_core_util_stats(coreid_bcyin_bcyout, coreid_s_tin_tout_pktn_repn)

  trins, trcounts, totrcnts, cumrcnts = \
	analyze_tracereads(ddtup_tr, coreid_s_tin_tout_pktn_repn, trace_reader_ts_ids)

  trdata = (ddtup_tr, trins, trcounts, totrcnts, cumrcnts)

  state_dump_data(coreid_s_tin_tout_pktn_repn, stlabels,\
	coreid_wcyin_wcyout, coreid_bcyin_bcyout, core_utils)


  if cmd == 'st':
    return (ddtup, [], [], ddtup_w, trdata)

  elif cmd == 'st-detailed':
    #allwins, tin, tout = state_extract_packet_windows(ddtup, stin, stout)
    #state_stats, last_tmin = state_completion_time_stats(allwins, tin, tout)
    #state_pickledepickle_stat_data(state_stats)
    #state_stats = state_proc_time_cdf(state_stats)
    #state_genplots(state_stats)
    #lastwin_waitwins = state_waitwins(ddtup,ddtup_w,True, last_tmin)
    #return (ddtup,allwins,lastwin_waitwins, ddtup_w, trdata)
    waitwins = state_waitwins(ddtup,ddtup_w)
    return (ddtup, [], waitwins, ddtup_w, trdata)

  else:
    print 'bad command, should not get here'
    sys.exit(1)



