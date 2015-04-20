'''
Atomix project, pyparse_preproc.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sys

def preproc_stax(tf):
  f = open(tf, 'r')
  sls = [line.split()[1:] for line in f.readlines()]
  f.close()


  if len(sls[0]) == 6:
    dd = [(dnum,idv,ts) for (ig1, dnum, ig2, idv, ig3, ts) in sls]
  elif len(sls[0]) == 5:
    dd = [(dnum,idv,ts) for (dnum, ig2, idv, ig3, ts) in sls]
  else:
    print 'bad format'
    sys.exit(1)
  dd_st = [(dnum,idv,ts) for (dnum,idv,ts) in dd if int(idv) < 1000]
  dd_ax = [(dnum,idv,ts) for (dnum,idv,ts) in dd if int(idv) >= 1000]

  dd0 = [(idv,float(ts)) for (dnum,idv,ts) in dd_st if dnum == '0']
  dd1 = [(idv,float(ts)) for (dnum,idv,ts) in dd_st if dnum == '1']
  dd2 = [(idv,float(ts)) for (dnum,idv,ts) in dd_st if dnum == '2']
  dd3 = [(idv,float(ts)) for (dnum,idv,ts) in dd_st if dnum == '3']

  tup_st = (dd0, dd1, dd2, dd3)

  dd0 = [(idv,float(ts)) for (dnum,idv,ts) in dd_ax if dnum == '0']
  dd1 = [(idv,float(ts)) for (dnum,idv,ts) in dd_ax if dnum == '1']
  dd2 = [(idv,float(ts)) for (dnum,idv,ts) in dd_ax if dnum == '2']
  dd3 = [(idv,float(ts)) for (dnum,idv,ts) in dd_ax if dnum == '3']

  tup_ax = (dd0, dd1, dd2, dd3)
  return (tup_st, tup_ax)

def preproc(tf):
  f = open(tf, 'r')
  sls = [line.split()[1:] for line in f.readlines()]
  f.close()

  if len(sls) == 0:
    print 'No data in trace file'
    sys.exit(1)


  if len(sls[0]) == 6:
    dd = [(dnum,idv,ts) for (ig1, dnum, ig2, idv, ig3, ts) in sls]
  elif len(sls[0]) == 5:
    dd = [(dnum,idv,ts) for (dnum, ig2, idv, ig3, ts) in sls]
  else:
    print 'bad format'
    sys.exit(1)

  dd0 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '0']
  dd1 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '1']
  dd2 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '2']
  dd3 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '3']

  return (dd0, dd1, dd2, dd3)

def preproc_runs(tf, markin, markout):
  f = open(tf, 'r')
  sls = [line.split()[1:] for line in f.readlines()]
  f.close()


  if len(sls[0]) == 6:
    dd = [(dnum,idv,ts) for (ig1, dnum, ig2, idv, ig3, ts) in sls]
  elif len(sls[0]) == 5:
    dd = [(dnum,idv,ts) for (dnum, ig2, idv, ig3, ts) in sls]
  else:
    print 'bad format'
    sys.exit(1)


  dd_to_keep = []
  dd_run = []
  dd_runs = {}
  n_runs = 0
  run_on = 0
  run_lengths = []
  tin = 0
  tout = 0
  #markin is inclusive, markout is not
  for (dnum, idv, ts) in dd:
    if idv == markin and run_on == 0:
      run_on = 1
      tin = ts
    if idv == markout and run_on == 1:
      run_on = 0
      dd_to_keep.extend(dd_run)
      n_runs += 1
      dd_runs[n_runs] = dd_run
      dd_run = []
      tout = ts
      run_lengths.append(float(tout) - float(tin))
    if run_on:
      dd_run.append((dnum,idv,ts))

  dd = dd_to_keep

  dd0 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '0']
  dd1 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '1']
  dd2 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '2']
  dd3 = [(idv,float(ts)) for (dnum,idv,ts) in dd if dnum == '3']

  return n_runs, run_lengths, dd_runs, (dd0, dd1, dd2, dd3)
