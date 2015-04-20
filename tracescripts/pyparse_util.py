'''
Atomix project, pyparse_util.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import numpy as np
import matplotlib


matplotlib.rcParams.update({'font.size': 18})

def stats(a):
  a_mean = np.mean(a)
  a_std = np.std(a)
  a_max = max(a)
  a_min = min(a)
  a_diff = a_max - a_min
  a_median = np.median(a)
  a_len = len(a)
  a_pctl95 = np.percentile(a, 95)
  return (a_len, a_mean, a_std, a_min, a_max, a_diff, a_median, a_pctl95)

def sorted_interval_match(xxc, bkc):

  print '--'
  for v in xxc:
    print v
  print '--'
  for v in bkc:
    print v

  out = []
  bkit = iter(bkc)
  bk = bkit.next()
  (_, bkin), (_, bkout) = bk
  for xx in xxc:
    (_, xxin), (_, xxout) = xx
    match = bkin <= xxin and bkout >= xxout
    breaking = False
    while not match:
      try:
	bk = bkit.next()
      except StopIteration:
        print 'WARNING: not containing bucket found for interval, skipping...'
	breaking = True
	break
      (_, bkin), (_, bkout) = bk
      match = bkin <= xxin and bkout >= xxout
    if breaking:
      break
    if not match:
      print 'no interval match found'
      sys.exit(1)
    else:
      out.append((xx, bk))

  return out

