'''
Atomix project, _util.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import logging

#from optparse import OptionParser
from optparse import (OptionParser,BadOptionError,AmbiguousOptionError)

class MyOptionParser(OptionParser):
  def error(self, msg):
    pass



class PassThroughOptionParser(OptionParser):
    """
    An unknown option pass-through implementation of OptionParser.

    When unknown arguments are encountered, bundle with largs and try again,
    until rargs is depleted.  

    sys.exit(status) will still be called if a known argument is passed
    incorrectly (e.g. missing arguments or bad argument types, etc.)        
    """
    def _process_args(self, largs, rargs, values):
        while rargs:
            try:
                OptionParser._process_args(self,largs,rargs,values)
            except (BadOptionError,AmbiguousOptionError), e:
                largs.append(e.opt_str)


def print_line():
  print '----------------------------------------------------------------'

def confirm(IS_INTERACTIVE=False):
  if IS_INTERACTIVE:
    print 'Ok?'
    raw_input()

def create_logger(LOGFILE):
  # create logger with 'pxc'
  logger = logging.getLogger('pxc')
  #logger.setLevel(logging.INFO)
  logger.setLevel(logging.DEBUG)

  # create file handler which logs even debug messages
  fh = logging.FileHandler(LOGFILE, mode='w')
  #fh = logging.FileHandler(LOGFILE, mode='a')
  #fh.setLevel(logging.INFO)
  fh.setLevel(logging.DEBUG)

  # create console handler with a higher log level
  ch = logging.StreamHandler()
  ch.setLevel(logging.INFO)

  # create formatter and add it to the handlers
  formatter = logging.Formatter('%(asctime)s:%(name)s:%(levelname)s:%(message)s')
  fh.setFormatter(formatter)
  ch.setFormatter(formatter)

  # add the handlers to the logger
  logger.addHandler(fh)
  logger.addHandler(ch)

  return logger
