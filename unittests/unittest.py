'''
Atomix project, unittest.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sys
import os

#TI code runner script
#
#Input: Bin file name, Target configuration file
#Action: Load the bin file and run it
#Return: Action status

class CodeRunner:
  def runCode(binary_path, session_ids):
    session_ids = ['ds0', 'ds1', 'ds2', 'ds3']
    print 'coderunner'
    print os.getcwd(), binary_path
    binary_path = os.path.abspath(binary_path)
    cmd_prog = 'esh prog "%s"' % binary_path
    print cmd_prog
    os.system(cmd_prog)
    for session_id in session_ids:
      cmd_halt = 'esh halt %s' % session_id
      cmd_load = 'esh load %s' % session_id
      #cmd_restart = 'esh restart %s' % session_id
      cmd_run = 'esh run %s' % session_id
      print cmd_halt
      print cmd_load
      #print cmd_restart
      print cmd_run
      os.system(cmd_halt)
      os.system(cmd_load)
      #os.system(cmd_restart)
      os.system(cmd_run)
      break
    return 0

def main():

  print 'unittest.py success'
  print '+++++++++++++'
  print sys.argv[1]
  print '+++++++++++++'

  binary_path = sys.argv[1]


  stat = CodeRunner.runCode(binary_path, '')

  if stat == 0:
   status = 'success'
  else:
    status = 'failed'

  print sys.argv[0], status

  #print '------------------------'
  #print os.environ
  #print '------------------------'

  sys.exit(stat)


if __name__ == "__main__":
  main()
