'''
Atomix project, _linker_conf.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

class MemorymapGenerator:

  def create_memorymap_output(self, sym_list_L1P, memorymap_input, memorymap_output, HAVE_L1PSRAM):
    fn = memorymap_input
    fnt = "".join(fn.split('.cmd')[:-1]) + "_template.cmd"
    self.logger.info(fnt)
    f = open(fnt, 'r')
    l = f.readlines()
    f.close()

    f = open(memorymap_output, 'w')
    for line in l:
      f.write(line)

    if (HAVE_L1PSRAM > 0):	#16 or 32
      for sym in sym_list_L1P:
	sec_out = '.text:' + sym + ' > L1PSRAM\n'
	f.write(sec_out)

    f.write('}')
    f.close()

  def __init__(self, logger):
    self.logger = logger
