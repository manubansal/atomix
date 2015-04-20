'''
Atomix project, pyparse_axns.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

def parse_axns_single(dd):
  entryidxs = range(0,len(dd),2)
  exitidxs = range(1,len(dd),2)
  ddentry = [dd[i] for i in entryidxs]
  ddexit = [dd[i] for i in exitidxs]
  
  #validate and process trace
  durs = {}
  for dentry, dexit in zip(ddentry, ddexit):
    (idv1,ts1) = dentry
    (idv2,ts2) = dexit

    if idv1 != idv2:
      print "ERROR: trace id pair mismatch"
      sys.exit(1)

    if idv1 in durs:
      durs[idv1].append(ts2 - ts1) 
    else:
      durs[idv1] = [ts2 - ts1]

  all_stats = {}
  for axnid in durs:
    all_stats[axnid] = stats(durs[axnid])

  return all_stats, durs

  

def parse_axns(tf, ddtup):
  (dd0, dd1, dd2, dd3) = ddtup

  tfb = tf.split('.')[0]
  axn_table_f = '%s_axntable.txt' % tfb
  f = open(axn_table_f,'r')
  axn_labels={}
  for line in f.readlines():
    (axnid,label) = [x.strip() for x in line.split(',')]
    axn_labels[axnid] = label
    #print (axnid, label)
  f.close()
  
  axn_stats_0, durs_0 =  parse_axns_single(dd0)
  axn_stats_1, durs_1 =  parse_axns_single(dd1)
  axn_stats_2, durs_2 =  parse_axns_single(dd2)
  axn_stats_3, durs_3 =  parse_axns_single(dd3)

  axn_stats = []
  axn_stats.extend(axn_stats_0.items())
  axn_stats.extend(axn_stats_1.items())
  axn_stats.extend(axn_stats_2.items())
  axn_stats.extend(axn_stats_3.items())

  #sort by standard deviation of compute time
  axn_stats_sorted = sorted(axn_stats, key=lambda st: st[1][1])
  #for row in axn_stats_sorted:
  #  print row

  axn_stats_sorted_inc_std = sorted(axn_stats, key=lambda st: st[1][2])
  for row in axn_stats_sorted_inc_std:
    print row


  print 'normalized action stats'
  axn_stats_normalized = [(axn_id, tuple([x/axn_stats_one[1] for x in axn_stats_one])) for (axn_id, axn_stats_one) in axn_stats_sorted]
  axn_stats_n = []
  for axn_stat_norm_one, axn_stat_one in zip(axn_stats_normalized, axn_stats_sorted):
    axn_id, (cnt, mean, std, minn, maxx, diff, median, pctl95) = axn_stat_norm_one
    ig, (cnto,_,_,_,_,_,_,_)  = axn_stat_one
    axn_stats_n.append((axn_id, (cnto, mean, std, minn, maxx, diff, median, pctl95)))
  axn_stats_normalized = axn_stats_n

  for row in axn_stats_normalized:
    print row

  axn_labels_sorted = []
  for axnid,_ in axn_stats_sorted:
    axn_labels_sorted.append(axn_labels[axnid])

  all_durs = {}
  all_durs.update(durs_0) 
  all_durs.update(durs_1) 
  all_durs.update(durs_2) 
  all_durs.update(durs_3)

  axn_labels_dict = axn_labels
  return axn_stats_sorted, axn_labels_sorted, axn_stats_normalized, all_durs, axn_labels_dict  #same_order

def plot_axn_timeseries(tf, axn_stats_norm, all_durs, axn_labels_dict):
  tfname =  '%s_plot_actions_timeseries.png' % tf
  worst_how_many = 3
  fs = ['r.-','g.-','b.-']

  #re-sort by normalized max
  axn_stats_norm_resorted = sorted(axn_stats_norm, key=lambda st: st[1][4], reverse=True)

  worst_top = []
  worst_top_labels = []
  for axn_stat in axn_stats_norm_resorted[0:worst_how_many]:
    axn_id, (cnt, mean, std, minn, maxx, diff, median, pctl95) = axn_stat
    worst_top.append(axn_id)
    worst_top_labels.append(axn_labels_dict[axn_id])
    print axn_labels_dict[axn_id]

  plt.figure()
  plt.margins(0.1)	#extend axis limits by this fraction
  plt.subplots_adjust(left=0.15, right=0.9, bottom=0.2, top=0.85)
  plt.title('Action compute times timeseries')
  for axn_id, f in zip(worst_top, fs):
    plt.plot(all_durs[axn_id], f)
    #plt.plot(range(1,1+len(all_durs[axn_id])), [int(t) for t in all_durs[axn_id]], f)
    #plt.plot(range(1,1+len(all_durs[axn_id])), [int(t) for t in all_durs[axn_id]], '.-')






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
  ax.set_position([box.x0, box.y0 + box.height * 0.1,
		   box.width, box.height * 0.9])
  # Put a legend below current axis
  #ax.legend(worst_top_labels, loc='upper center', bbox_to_anchor=(0.5, -0.05))#,
  ax.legend(worst_top_labels, loc='upper center', bbox_to_anchor=(0.5, -0.13))#,
           # fancybox=True, shadow=True, ncol=5)

  #plt.show()
  plt.savefig(tfname)

def plot_axns_i(tfname, axn_stats_sorted, axn_labels_sorted):
  axnids = [axnid for (axnid, stat_tup) in axn_stats_sorted]
  stat_tups = [stat_tup for (axnid, stat_tup) in axn_stats_sorted]
  means   = [mean   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  medians = [median for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  mins    = [minn   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  maxs    = [maxx   for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]
  pctls95 = [pctl95 for (cnt, mean, std, minn, maxx, diff, median, pctl95) in stat_tups]

  plt.figure()
  plt.margins(0.1)	#extend axis limits by this fraction
  plt.subplots_adjust(left=0.15, right=0.75, bottom=0.1, top=0.85)
  plt.title('Action compute times and predictability')

  #plt.plot(c0_ts, c0_st, 'r', c1_ts, c1_st, 'g', c2_ts, c2_st, 'b', c3_ts, c3_st, 'k')
  ids = range(len(axnids))
  plt.plot(means   ,ids,  'b.-')
  plt.plot(maxs    ,ids,  'r.-')
  plt.plot(mins    ,ids,  'g.-')
  plt.plot(medians ,ids,  'k.--')
  plt.plot(pctls95 ,ids,  'k.-')

  xs = ids
  #ys = maxs
  ys = means
  ats = axn_labels_sorted
  for x, y, at in zip(xs, ys, ats):
    #plt.annotate(at, xy=(x,y), rotation=60)
    plt.annotate(at, xy=(y*1.4,x), rotation=0, fontsize='x-small')

  #plt.xlabel('Action ID')
  #plt.ylabel('Cycles')
  plt.ylabel('Action ID')
  plt.xlabel('Cycles')
  plt.grid(True)
  plt.legend(['mean', 'max', 'min', 'median','95%tile'], loc='lower right')
  #plt.show()
  plt.savefig(tfname)
  #plt.savefig('%s_plot_states.png' % tf, pad_inches=2)


def plot_axns(tf, axn_stats_sorted, axn_labels, axn_stats_norm):
  tfname =  '%s_plot_actions.png' % tf
  plot_axns_i(tfname, axn_stats_sorted, axn_labels)

  tfname =  '%s_plot_actions_norm.png' % tf
  plot_axns_i(tfname, axn_stats_norm, axn_labels)
