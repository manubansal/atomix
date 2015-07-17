
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
