'''
Atomix project, plot_atoms.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sys, os
from pyparse_util import *
import matplotlib
import matplotlib.pyplot as plt

FIGSIZE_PLOT_ATOMS = (70,70)

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
  plt.xlabel('Atom index')
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

  tfname_pdf = tfname + ".pdf"
  plt.savefig(tfname_pdf)

  plt.title('Atom compute times and predictability\n%s' % os.getcwd().split('orsys')[1])
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
