'''
Atomix project, plot_states.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import pickle
import os
import sys
from matplotlib.patches import Rectangle
import math
from pyparse_util import sorted_interval_match

def state_genplots(state_stats):
  _,_,completion_times,csum,bin_edges = state_stats
  #------------------------------------------------------------------
  plt.figure()
  plt.figure(figsize=(20, 7))
  plt.plot(bin_edges[1:], csum)
  plt.ylabel('CDF')
  plt.xlabel('Completion time (cycles)')
  plt.title('#pkts: %d' % len(completion_times))
  plt.grid(True)
  #plt.savefig('%s_plot_cdf.png' % tf);
  plt.savefig('plot_cdf.png') 
  #------------------------------------------------------------------

def plot_tail_symbol_stats(tf, allwins, conf):

  tail_state = conf['tail_state']
  fini_state = conf['fini_state']

  tail_sym_latencies = []
  tail_sym_latencies_0 = []
  tail_sym_latencies_1 = []
  tail_sym_latencies_2 = []
  tail_sym_latencies_3 = []

  for win in allwins:
    dd0 = [d0 for d0, d1, d2, d3 in win]
    dd1 = [d1 for d0, d1, d2, d3 in win]
    dd2 = [d2 for d0, d1, d2, d3 in win]
    dd3 = [d3 for d0, d1, d2, d3 in win]

    c0_ts = [ts for (st, ts) in dd0]
    c0_st = [st for (st, ts) in dd0]

    #c1_ts = [ts for (st, ts) in dd1]
    #c1_st = [st for (st, ts) in dd1]

    #c2_ts = [ts for (st, ts) in dd2]
    #c2_st = [st for (st, ts) in dd2]

    c3_ts = [ts for (st, ts) in dd3]
    c3_st = [st for (st, ts) in dd3]


    for (st, ts) in dd0:
      if st == tail_state:
	c0_ts_tail_entry = ts
	break;

    for (st, ts) in dd0:
      if st == fini_state:
	c0_ts_fini_exit = ts
    #---

    for (st, ts) in dd1:
      if st == tail_state:
	c1_ts_tail_entry = ts
	break;

    for (st, ts) in dd1:
      if st == fini_state:
	c1_ts_fini_exit = ts
    #---

    for (st, ts) in dd2:
      if st == tail_state:
	c2_ts_tail_entry = ts
	break;

    for (st, ts) in dd2:
      if st == fini_state:
	c2_ts_fini_exit = ts
    #---

    for (st, ts) in dd3:
      if st == tail_state:
	c3_ts_tail_entry = ts
	break;

    for (st, ts) in dd3:
      if st == fini_state:
	c3_ts_fini_exit = ts
    #---


    tail_sym_latency = c3_ts_fini_exit - c0_ts_tail_entry
    tail_sym_latency_0 = c0_ts_fini_exit - c0_ts_tail_entry
    tail_sym_latency_1 = c1_ts_fini_exit - c1_ts_tail_entry
    tail_sym_latency_2 = c2_ts_fini_exit - c2_ts_tail_entry
    tail_sym_latency_3 = c3_ts_fini_exit - c3_ts_tail_entry

    tail_sym_latencies.append(tail_sym_latency)
    tail_sym_latencies_0.append(tail_sym_latency_0)
    tail_sym_latencies_1.append(tail_sym_latency_1)
    tail_sym_latencies_2.append(tail_sym_latency_2)
    tail_sym_latencies_3.append(tail_sym_latency_3)

  tail_sym_latency_mean = np.mean(tail_sym_latencies)
  tail_sym_latency_std = np.std(tail_sym_latencies)
  tail_sym_latency_max = max(tail_sym_latencies)
  tail_sym_latency_min = min(tail_sym_latencies)
  tail_sym_latency_diff = tail_sym_latency_max - tail_sym_latency_min

  tail_sym_latency_0_mean = np.mean(tail_sym_latencies_0)
  tail_sym_latency_0_diff = max(tail_sym_latencies_0) - min(tail_sym_latencies_0)
  tail_sym_latency_1_mean = np.mean(tail_sym_latencies_1)
  tail_sym_latency_1_diff = max(tail_sym_latencies_1) - min(tail_sym_latencies_1)
  tail_sym_latency_2_mean = np.mean(tail_sym_latencies_2)
  tail_sym_latency_2_diff = max(tail_sym_latencies_2) - min(tail_sym_latencies_2)
  tail_sym_latency_3_mean = np.mean(tail_sym_latencies_3)
  tail_sym_latency_3_diff = max(tail_sym_latencies_3) - min(tail_sym_latencies_3)

  width=0.8
  plt.figure(figsize=(10,10))
  #plt.bar(bincenters,y,width=width, color='r', yerr=menStd)
  #y = [tail_sym_latency_min, tail_sym_latency_mean, tail_sym_latency_max]
  #ystd = tail_sym_latency_std
  #ydiff = tail_sym_latency_diff
  #ymean = [tail_sym_latency_mean]
  #ymin = [tail_sym_latency_min]
  #ymax = [tail_sym_latency_max]
  #plt.bar([1, 2, 3],y, width=width, color='r', yerr=ystd)

  #xmin = [1,2]
  #xmean = [x + width for x in xmin]
  #xmax = [x + width for x in xmean]
  #plt.bar(xmin,ymin, width=width, color='g')
  #plt.bar(xmean,ymean, width=width, color='b', yerr=ystd)
  #plt.bar(xmax,ymax, width=width, color='r')

  ydiff = [tail_sym_latency_diff, tail_sym_latency_0_diff, tail_sym_latency_1_diff, tail_sym_latency_2_diff, tail_sym_latency_3_diff]
  ymean = [tail_sym_latency_mean, tail_sym_latency_0_mean, tail_sym_latency_1_mean, tail_sym_latency_2_mean, tail_sym_latency_3_mean]



  x = [-1.4,-0.4,0.6,1.6,2.6]
  y = ymean
  ys = tail_sym_latencies
  errx = [v + 0.4 for v in x]
  erry = ydiff

  pltData = [x, y, errx, erry, ys]
  print os.getcwd()
  tag = os.getcwd().split('trace')[1].split('/')[-1]
  print tag
  fname = '../hist_stats.pkl'
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
  print 'Saved plot data to %s' % fname


  plt.bar(x,y, width=width, color='g', yerr=erry)

  #(_, caps, _) = plt.errorbar(errx, ymean, ydiff, capsize=20, elinewidth=3, linestyle='.')
  (_, caps, _) = plt.errorbar(errx, y, erry, capsize=10, elinewidth=6, linestyle='.', ecolor='black')

  for cap in caps:
      cap.set_color('black')
      cap.set_markeredgewidth(3)

  plt.xlabel('Core ID (-1 is for the total symbol latency)')
  #plt.xticks(['Net','Core1','Core2','Core3','Core4'])
  plt.ylabel('Cycles')
  plt.title('Mean tail symbol latency (err: [min, max])')
  plt.savefig('%s_plot_hist.png' % tf)

  plt.figure()
  plt.plot(ys, '.-g')
  plt.savefig('%s_plot_tail_scatter_timeorder.png' % tf)

  ys = np.abs(np.fft.fft(tail_sym_latencies)) ** 2
  plt.figure()
  #plt.plot(ys, 'r')
  plt.plot(ys[1:], 'r')	#discard dc since it hides rest of it
  plt.savefig('%s_plot_tail_scatter_fft.png' % tf)

  ys = tail_sym_latencies
  ys.sort()
  plt.figure()
  plt.plot(ys, '.')
  plt.savefig('%s_plot_tail_scatter.png' % tf)


def plot_joint_states(tf, tfst):
  f = open(tf, 'rb')
  d = pickle.load(f)
  f.close()
  print d

  f = open(tfst, 'rb')
  dst = pickle.load(f)
  f.close()
  print dst



  colors = {'test1-st':'c','test2-st':'r','test3-st':'y','test5-st':'m', 'test6-st':'g','test8-st':'b'}
  hatches = {'test1-st':'/','test2-st':'\\','test3-st':'.','test5-st':'x', 'test6-st':'+', 'test8-st':'-'}
  cases= [1,2,3,5,6,8]
  tags_ord = ['test%d-st' % v for v in cases]

  #labels = ['Scheme%d' % v for v in cases]
  labels = ['1) L1P all cache, L1D all cache',\
  	    '2) L1P all cache, L1D disabled',\
	    '3) L1P disabled , L1D disabled',\
	    '5) L1P all cache, L1D all SRAM',\
	    '6) L1P all cache, L1D 28K SRAM',\
	    '8) L1P all cache, L1D all SRAM/stack in L1D']
	    

  bar_array=[]


#------------------------------
# tail sym histogram
#------------------------------
  #width=0.2
  width=0.15
  offset = 0
  plt.figure(figsize=(15,10))
  for tag in tags_ord:
    [x, y, errx, erry, ys] = d[tag]
    x = [v + offset for v in x]
    #errx = [v + offset for v in errx]
    errx = [v + (width/2) for v in x]
    offset += width

    if tag in colors:
      c = colors[tag]
    else:
      c = 'k'
    h = hatches[tag]
    b = plt.bar(x,y, width=width, color=c, yerr=erry, hatch=h)
    bar_array.append(b)

    #(_, caps, _) = plt.errorbar(errx, ymean, ydiff, capsize=20, elinewidth=3, linestyle='.')
    (_, caps, _) = plt.errorbar(errx, y, erry, capsize=5, elinewidth=4, linestyle='.', ecolor='black')

    for cap in caps:
	cap.set_color('black')
	cap.set_markeredgewidth(2)

  plt.legend(bar_array, labels)
  plt.xlabel('Core ID (-1 is for the total symbol latency)')
  #plt.xticks(['Net','Core1','Core2','Core3','Core4'])
  plt.xticks([-1,0,1,2,3])
  plt.ylabel('Cycles')
  plt.title('Mean tail symbol latency (err: [min, max])')

  outfname = '%s_plot_hist.png' % tf
  print 'saving to %s' % outfname
  plt.savefig(outfname)


#------------------------------
# state histogram
#------------------------------
  width = 0.8
  offset = 0
  plt.figure(figsize=(10,10))
  plt.subplots_adjust(left=0.15, right=0.9, bottom=0.05, top=0.95)
  xs = range(len(cases))
  for tag, x in zip(tags_ord, xs):
    [y, erry, ys] = dst[tag]
    x = [x]
    x = [v + offset for v in x]
    #errx = [v + offset for v in errx]
    errx = [v + (width/2) for v in x]
    offset += width

    if tag in colors:
      c = colors[tag]
    else:
      c = 'k'
    h = hatches[tag]
    b = plt.bar(x,y, width=width, color=c, yerr=erry, hatch=h)
    bar_array.append(b)

    #(_, caps, _) = plt.errorbar(errx, ymean, ydiff, capsize=20, elinewidth=3, linestyle='.')
    (_, caps, _) = plt.errorbar(errx, y, erry, capsize=5, elinewidth=4, linestyle='.', ecolor='black')

    plt.annotate('%0.2f%%' % (erry * 100/y), xy=(x[0], y + 3* erry))

    for cap in caps:
	cap.set_color('black')
	cap.set_markeredgewidth(2)


  plt.xlabel('Various memory configurations')
  #plt.xticks(['Net','Core1','Core2','Core3','Core4'])
  plt.xticks([])
  plt.ylabel('Cycles')
  plt.title('Mean packet compute time (err: [min, max])')

  #plt.legend(bar_array, labels)
  #plt.legend(bar_array, labels, loc='upper left')
  #------
  ## Shink current axis by 25% in width
  #ax = plt.subplot(111)
  #box = ax.get_position()
  #ax.set_position([box.x0, box.y0, box.width * 0.75, box.height])
  ## Put a legend to the right of the current axis
  ##plt.legend(bar_array, labels)
  #ax.legend(bar_array, labels, loc='center left', bbox_to_anchor=(1, 0.5))
  #------
  # Shink current axis by 25% in height
  ax = plt.subplot(111)
  box = ax.get_position()
  #ax.set_position([box.x0, box.y0, box.width, box.height * 0.65])
  ax.set_position([box.x0, box.y0 + box.height*0.30, box.width, box.height * 0.70])
  # Put a legend to the bottom of the current axis
  #plt.legend(bar_array, labels)
  #ax.legend(bar_array, labels, loc='lower center', bbox_to_anchor=(0.5, 0))
  ax.legend(bar_array, labels, loc='upper center', bbox_to_anchor=(0.5, -0.05))


  outfname = '%s_plot_st_hist.png' % tfst
  print 'saving to %s' % outfname
  plt.savefig(outfname)


#-------------------------
def plot_states_pdf(tf, alldd, state_profiling_table_file="", waitwins=[], conf={}, alldd_w=[], trdata=([],[],[])):
#-------------------------
  fig = plt.figure(figsize=(10,8), dpi=1200)
  ax = fig.add_subplot(111)
  axR = plt.subplot(1,1,1, sharex=ax, frameon=False)
  axR.yaxis.tick_right()
  axR.yaxis.set_label_position("right")

  #----------------------------------------
  ### read and prepare configuration
  #----------------------------------------
  ofdm_sym_dur_us 	= conf['ofdm_sym_dur_us']
  clock_rate_mhz 	= conf['clock_rate_mhz']
  ssi 			= conf['sample_consumption_starts_in_state']
  sso 			= conf['sample_consumption_ends_in_state']
  spi 			= conf['rx_pkt_init_state']
  spo 			= conf['rx_data_finish_state']

  ddtup_tr, trins, trcounts, totrcnts, cumrcnts = trdata
  alldd_tr = ddtup_tr

  if state_profiling_table_file:
    f = open(state_profiling_table_file, 'r')
    state_profiling_table = [l.strip().split(',') for l in f.readlines()]
    f.close()

  dd0, dd1, dd2, dd3 = alldd

  print 'dd0:', dd0
  print 'dd3:', dd3

  c0_ts = [ts for (st, ts) in dd0]
  c0_st = [st for (st, ts) in dd0]

  c1_ts = [ts for (st, ts) in dd1]
  c1_st = [st for (st, ts) in dd1]

  c2_ts = [ts for (st, ts) in dd2]
  c2_st = [st for (st, ts) in dd2]

  c3_ts = [ts for (st, ts) in dd3]
  c3_st = [st for (st, ts) in dd3]

  ls = [[ts for st, ts in ddi if st == sso] for ddi in alldd]
  xmax = max([l[1] for l in ls])
  xmin = 0
  plt.xlim(xmin,xmax)

  all_states_visited = sorted([int(s) for s in set(c0_st + c1_st + c2_st + c3_st)])
  visited_states_enumeration = enumerate(all_states_visited)
  visited_states_dict = {}
  for i,s in visited_states_enumeration:
    visited_states_dict[s] = i
  vsd = visited_states_dict
  #print all_states_visited
  #print visited_states_enumeration
  #print visited_states_dict
  #raw_input()

  dy = 0.2

  c0_st = [float(vsd[int(s)]) + 0 * dy for s in c0_st] 
  c1_st = [float(vsd[int(s)]) + 1 * dy for s in c1_st] 
  c2_st = [float(vsd[int(s)]) + 2 * dy for s in c2_st]
  c3_st = [float(vsd[int(s)]) + 3 * dy for s in c3_st]

  plt.plot(c0_ts, c0_st, 'rx-', c1_ts, c1_st, 'gx-', c2_ts, c2_st, 'bx-', c3_ts, c3_st, 'k.-')

  xmin, xmax = ax.get_xlim()
  xmin = 0
  xmax = int(math.ceil(xmax))

  ts_max = xmax + 1000
  state_ids = []
  yy = []
  lbls = []
  for e in state_profiling_table:
    id,lbl = e
    id = int(id)
    if not id in vsd:
      continue
    id = vsd[id]
    state_ids.append(id)
    xy = (ts_max, id + dy)
    print xy

    #plt.annotate(lbl.strip(), xy)

    yy.append(id)
    lbls.append("%2d %s" % (id, lbl))

  #plt.yticks(yy, lbls)
  axR.set_yticklabels(lbls)

  ymin, ymax = ax.get_ylim()


  #ax.axes.get_yaxis().set_visible(False)
  ax.set_frame_on(False)
  ax.get_xaxis().tick_bottom()

  plt.grid(True)

  ofdm_sym_dur_cyc 	= ofdm_sym_dur_us * clock_rate_mhz

  plt.yticks(state_ids)
  #ax.set_yticklabels(state_ids)

  plt.xlabel('Timestamp (cycle)')
  #plt.ylabel('State number')

  #add ofdm symbol markers
  xticks, lbls = plt.xticks()
  xticks = list(xticks)
  #lbls = [l.get_text() for l in lbls]
  lbls = ['%d' % int(t) for t in xticks]
  print xticks
  print lbls
  plt.legend(['dsp0', 'dsp1', 'dsp2', 'dsp3'], loc='upper left')

  xmin -= ofdm_sym_dur_cyc
  plt.xlim(xmin,xmax)

  #plt.margins(0.5)	#extend axis limits by this fraction

  plt.tight_layout()
  plt.subplots_adjust(left=0.07, right=0.55, bottom=0.18, top=0.98, wspace=0, hspace=0)

  #rmargin = 100000
  #bmargin = 10000
  #x0, x1, y0, y1 = plt.axis()
  #plt.axis((x0, x1 + rmargin, y0, y1)) 


  locs, labels = plt.xticks()
  plt.setp(labels, rotation=30)

  plt.savefig('%s_plot_states.png.pdf' % tf)


#-------------------------
def plot_states(tf, alldd, state_profiling_table_file="", waitwins=[], conf={}, alldd_w=[], trdata=([],[],[])):
#-------------------------
  plot_states_pdf(tf, alldd, state_profiling_table_file, waitwins, conf, alldd_w, trdata)
  return

  print conf

  ddtup_tr, trins, trcounts, totrcnts, cumrcnts = trdata
  alldd_tr = ddtup_tr

  if state_profiling_table_file:
    f = open(state_profiling_table_file, 'r')
    state_profiling_table = [l.strip().split(',') for l in f.readlines()]
    f.close()


  dd0, dd1, dd2, dd3 = alldd

  print 'dd0:', dd0
  print 'dd3:', dd3

  c0_ts = [ts for (st, ts) in dd0]
  c0_st = [st for (st, ts) in dd0]

  c1_ts = [ts for (st, ts) in dd1]
  c1_st = [st for (st, ts) in dd1]

  c2_ts = [ts for (st, ts) in dd2]
  c2_st = [st for (st, ts) in dd2]

  c3_ts = [ts for (st, ts) in dd3]
  c3_st = [st for (st, ts) in dd3]

  all_states_visited = sorted([int(s) for s in set(c0_st + c1_st + c2_st + c3_st)])
  visited_states_enumeration = enumerate(all_states_visited)
  visited_states_dict = {}
  for i,s in visited_states_enumeration:
    visited_states_dict[s] = i
  vsd = visited_states_dict
  #print all_states_visited
  #print visited_states_enumeration
  #print visited_states_dict
  #raw_input()

  fig = plt.figure(figsize=conf['figsize'])
  ax = fig.add_subplot(111)
  plt.margins(0.1)	#extend axis limits by this fraction
  plt.subplots_adjust(left=0.1, right=0.75, bottom=0.1, top=0.85)
  plt.title('State transition timing diagram')
  dy = 0.2

  c0_st = [float(s) + 0 * dy for s in c0_st] 
  c1_st = [float(s) + 1 * dy for s in c1_st] 
  c2_st = [float(s) + 2 * dy for s in c2_st]
  c3_st = [float(s) + 3 * dy for s in c3_st];

  plt.plot(c0_ts, c0_st, 'r.-', c1_ts, c1_st, 'g.-', c2_ts, c2_st, 'b.-', c3_ts, c3_st, 'k.-')

  cx_ts_st_pairs = ((c0_ts,c0_st),(c1_ts,c1_st),(c2_ts,c2_st),(c3_ts,c3_st))

  xmin, xmax = ax.get_xlim()
  xmin = 0
  xmax = int(math.ceil(xmax))
  #xmax = 250000
  #xmax = 500000

  #add ofdm symbol markers
  #ax.xaxis.set_ticks(range(45000,130000,5000))
  #xticks_extra = range(45000,200000,5000)
  ####xticks_extra = range(xmin,xmax,5000)
  ####labels_extra = [""] * len(xticks_extra)



  #plt.axvline(x=c0_ts_tail_entry, ls='--')
  #plt.axvline(x=c3_ts_fini_exit, ls='--')
  #plt.annotate('Tail sym latency: %dcy' % tail_sym_latency, xy=(c3_ts_fini_exit + 200, 10.5))

  ts_max = max(c3_ts) + 1000
  state_ids = []
  for e in state_profiling_table:
    id,lbl = e
    id = int(id)
    state_ids.append(id)
    xy = (ts_max, id + dy)
    plt.annotate(lbl, xy)

  ymin, ymax = ax.get_ylim()


  #----------------------------------------
  ### read and prepare configuration
  #----------------------------------------
  ltf_proc_entry_state 	= conf['ltf_proc_entry_state']
  ofdm_sym_dur_us 	= conf['ofdm_sym_dur_us']
  clock_rate_mhz 	= conf['clock_rate_mhz']
  n_ofdm_syms_in_pkt 	= conf['n_ofdm_syms_in_pkt']
  n_ofdm_syms_in_stf 	= conf['n_ofdm_syms_in_stf']
  slack_us 		= conf['slack_us']
  ssi 			= conf['sample_consumption_starts_in_state']
  sso 			= conf['sample_consumption_ends_in_state']
  spi 			= conf['rx_pkt_init_state']
  prepad_us		= conf['prepad_dur_us']
  postpad_us		= conf['postpad_dur_us']
  tri 			= conf['traceReaderInTSID']
  tro 			= conf['traceReaderOutTSID']
  WAIT_WIN_THRESHOLD	= conf['WAIT_WIN_THRESHOLD']

  slack_cyc 		= slack_us * clock_rate_mhz
  ofdm_sym_dur_cyc 	= ofdm_sym_dur_us * clock_rate_mhz
  prepad_cyc		= prepad_us * clock_rate_mhz
  postpad_cyc		= postpad_us * clock_rate_mhz
  pkt_us 		= n_ofdm_syms_in_pkt * ofdm_sym_dur_us





  plt.yticks(state_ids)
  plt.xlabel('Timestamp (cycle)')
  plt.ylabel('State number')

  #add ofdm symbol markers
  xticks, lbls = plt.xticks()
  xticks = list(xticks)
  #lbls = [l.get_text() for l in lbls]
  lbls = ['%d' % int(t) for t in xticks]
  print '999999999999999999'
  print xticks
  print lbls
  plt.legend(['dsp0', 'dsp1', 'dsp2', 'dsp3'], loc='upper left')


  ###----------------------------------------
  ##### add smallest-packet region indicators
  ###----------------------------------------
  ###find pkt start points
  ##st_is = [i for i, (st,ts) in enumerate(dd0) if st == ltf_proc_entry_state]
  ##entry_is = st_is[::2]

  ###plot indicators
  ##width = ofdm_sym_dur_cyc * (n_ofdm_syms_in_pkt - n_ofdm_syms_in_stf) + slack_cyc
  ##ymin, ymax = ax.get_ylim()
  ##height = ymax - ymin
  ##currentAxis = plt.gca()
  ##for i in entry_is:
  ##  st, ts = dd0[i]
  ##  #print ts
  ##  someX, someY = ts, ymin 
  ##  currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor="grey"))

  #----------------------------------------
  ### add sample rx cycle indicators
  #----------------------------------------
  #ddtup_tr, trins, trcounts, totrcnts, cumrcnts = trdata
  #it = iter(cumrcnts)

  #find pkt start points
  #st_is = [i for i, (st,ts) in enumerate(dd0) if st == ssi] 
  #entry_is = st_is[::2]
  st_is_ = [i for i, (st,ts) in enumerate(dd0) if st == spi] 
  st_is_ = st_is_[::2]
  st_is = [i+2 for i in st_is_]
  entry_is = st_is

  xmin -= ofdm_sym_dur_cyc
  plt.xlim(xmin,xmax)


  #----------------------------
  #plot traceReader entry/exit
  #----------------------------
  plot_traceReader_entry_exit(alldd_tr, tri, tro, ymax)


  #---------------------------
  #annotate state transitions
  #---------------------------
  annotate_state_transitions(cx_ts_st_pairs, dy)


  #---------------
  #plot indicators
  #---------------
  yrange = (ymin, ymax)
  spcs = []
  spcs = plot_indicators(conf, cumrcnts, yrange, entry_is, alldd, spcs)

  #----------------
  #plot waitwins
  #----------------
  plot_waitwins(waitwins, dy, WAIT_WIN_THRESHOLD)


  spcs.append(xmax)
  xticks_extra = []
  for x1, x2 in zip(spcs[:-1], spcs[1:]):
    xticks_extra += range(x1,x2,5000)
  labels_extra = [""] * len(xticks_extra)
  #xticks += xticks_extra
  #lbls += labels_extra
  xticks = xticks_extra
  lbls = labels_extra
  print xticks
  print lbls
  plt.xticks(xticks, lbls)
  #plt.xticks(xticks)

  plt.grid(True)

  #plt.show()
  plt.savefig('%s_plot_states.png' % tf)
  #plt.savefig('%s_plot_states.png' % tf, pad_inches=2)

def plot_traceReader_entry_exit(alldd_tr, tri, tro, ymax):
  if alldd_tr:
    dd0_tr, _, _, _ = alldd_tr
    trti = [ts for st, ts in dd0_tr if st == tri]
    trto = [ts for st, ts in dd0_tr if st == tro]
    for t in trti:
      plt.plot([trti, trti], [0,ymax], 'k--', linewidth=0.7)

def annotate_state_transitions(cx_ts_st_pairs, dy):
  ((c0_ts,c0_st),(c1_ts,c1_st),(c2_ts,c2_st),(c3_ts,c3_st)) = cx_ts_st_pairs

  annotate_only_core0 = True
  annotate_state_entry_times = False
  #annotate_state_exit_times = True
  if not annotate_state_entry_times:
    tss_sts = [\
    	(c0_ts[1::2],c0_st[1::2]),\
    	(c1_ts[1::2],c1_st[1::2]),\
	(c2_ts[1::2],c2_st[1::2]),\
	(c3_ts[1::2],c3_st[1::2])]
  else:
    tss_sts = [(c0_ts,c0_st),(c1_ts,c1_st),(c2_ts,c2_st),(c3_ts,c3_st)]
  coreid=0
  for (ts,st) in tss_sts:
    for tsv,stv in zip(ts,st):
      xy = (tsv,int(stv) + coreid * dy + dy)
      plt.annotate("%06d" % tsv, xy, rotation='30', fontsize='x-small')
    if annotate_only_core0:
      break
    coreid += 1


def plot_waitwins(waitwins, dy, WAIT_WIN_THRESHOLD):
  if waitwins:
    print 'plotting waitwins...'
    dyy = 0.05
    print 'len_waitwins=',len(waitwins)
    if waitwins:
      coreid = 0
      for wins in waitwins:
	for annotated_win in wins:
	  (stateid,coreid),win=annotated_win
	  y = int(stateid) + coreid * dy + dyy
	  ((s_in, t_in), (s_out, t_out)) = win
	  if t_out - t_in > WAIT_WIN_THRESHOLD:
	    plt.plot([t_in, t_out], [y,y], 'm-', linewidth=3) 
	coreid += 1
  else:
    print 'no waitwins, skipping...'
    raw_input()

def plot_indicators(conf, cumrcnts, yrange, entry_is, alldd, spcs):
  (ymin, ymax) = yrange
  dd0, dd1, dd2, dd3 = alldd
  it = iter(cumrcnts)

  #----------------------------------------
  ### read and prepare configuration
  #----------------------------------------
  ltf_proc_entry_state 	= conf['ltf_proc_entry_state']
  ofdm_sym_dur_us 	= conf['ofdm_sym_dur_us']
  clock_rate_mhz 	= conf['clock_rate_mhz']
  n_ofdm_syms_in_pkt 	= conf['n_ofdm_syms_in_pkt']
  n_ofdm_syms_in_stf 	= conf['n_ofdm_syms_in_stf']
  slack_us 		= conf['slack_us']
  ssi 			= conf['sample_consumption_starts_in_state']
  sso 			= conf['sample_consumption_ends_in_state']
  spi 			= conf['rx_pkt_init_state']
  prepad_us		= conf['prepad_dur_us']
  postpad_us		= conf['postpad_dur_us']
  tri 			= conf['traceReaderInTSID']
  tro 			= conf['traceReaderOutTSID']
  WAIT_WIN_THRESHOLD	= conf['WAIT_WIN_THRESHOLD']

  slack_cyc 		= slack_us * clock_rate_mhz
  ofdm_sym_dur_cyc 	= ofdm_sym_dur_us * clock_rate_mhz
  prepad_cyc		= prepad_us * clock_rate_mhz
  postpad_cyc		= postpad_us * clock_rate_mhz
  pkt_us 		= n_ofdm_syms_in_pkt * ofdm_sym_dur_us

  pad_width = prepad_cyc
  #pkt_width = ofdm_sym_dur_cyc * (n_ofdm_syms_in_pkt)
  stf_width = ofdm_sym_dur_cyc * 2
  ltf_width = ofdm_sym_dur_cyc * 2
  plcp_width = ofdm_sym_dur_cyc 
  data_width = ofdm_sym_dur_cyc * (n_ofdm_syms_in_pkt - 5)
  slack_width = slack_cyc

  npkts = len(cumrcnts)
  pkt_start_pnts_us = [prepad_us]
  for i in range(0, npkts-1):
    pkt_start_pnts_us.append(pkt_start_pnts_us[-1] + pkt_us + postpad_us + prepad_us)
  print 'pkt_start_pnts_us = ', pkt_start_pnts_us
  pkt_start_pnts_us.append(0)
  it2 = iter(pkt_start_pnts_us)

  height = ymax - ymin
  currentAxis = plt.gca()
  pkt_start_pnt_us = next(it2)
  entry_is = entry_is[0:npkts]
  print 'entry_is = ', entry_is
  for i in entry_is:
    print pad_width
    st, ts = dd0[i]
    #print ts
    someX, someY = ts, ymin 
    someX -= ofdm_sym_dur_cyc	#since the first symbol should be available by the 
    				#time the pkt detect state is first entered and the
				#first ofdm symbol is read.
    start_pnt_cyc = someX
    spcs.append(start_pnt_cyc)

    width = pad_width; c = conf['c_prepad']
    currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor=c, edgecolor='none'))

    #currentAxis.add_patch(Rectangle((someX + pad_width, someY), \
    #	pkt_width, height, facecolor="blue"))
    someX += pad_width; width = stf_width; c = conf['c_stf']
    currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor=c, edgecolor='none'))

    someX += stf_width; width = ltf_width; c = conf['c_ltf']
    currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor=c, edgecolor='none'))

    someX += ltf_width; width = plcp_width; c = conf['c_plcp']
    currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor=c, edgecolor='none'))

    someX += plcp_width; width = data_width; c = conf['c_data']
    currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor=c, edgecolor='none'))

    someX += data_width; width = slack_width; c = conf['c_slack']
    currentAxis.add_patch(Rectangle((someX, someY), width, height, facecolor=c, edgecolor='none'))

    #samples_consumed_us = math.ceil((prepad_us + pkt_us) * 1.0/ofdm_sym_dur_us) * ofdm_sym_dur_us
    samples_consumed_us = next(it) * ofdm_sym_dur_us
    pkt_start_pnt_us = next(it2)
    residual_pad_us = pkt_start_pnt_us - samples_consumed_us
    pad_width = residual_pad_us * clock_rate_mhz
    print 'samples_consumed_us=%s, residual_pad_us=%s' % (samples_consumed_us, residual_pad_us)
  return spcs
