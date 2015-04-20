'''
Atomix project, check_ORILIB_PacketSearch_Align.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sys
import coderunner
import os

stat = coderunner.runCode('', '')

if stat == 0:
 status = 'success'
else:
  status = 'failed'

print sys.argv[0], status

print '------------------------'
print sys.argv[1]
print os.environ
print '------------------------'

sys.exit(stat)
