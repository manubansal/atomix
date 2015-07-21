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



def state_dump_data(stlabels, \
	coreid_wcyin_wcyout, coreid_bcyin_bcyout, core_utils):

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
