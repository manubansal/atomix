'''
Atomix project, _gencode_atoms.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from _db import *
from _codegen_write_out import *
from _util import *

class AtomsCodeGenerator:
  ############################################################################################
  #Create atoms from abstract declaration
  ############################################################################################

  def code_h_atoms_include(self, typename):
    code_h = ''
    code_h += '#include <__%s__.h>\n' % typename
    return code_h

  def code_h_atoms_declaration(self, typename, instancename):
    code_h = ''
    code_h += 'extern %s far *%s;\n' % (typename, instancename)
    return code_h

  def code_c_atoms_declaration(self, typename, instancename, coreid, optimized_atom):
    #-------------------------------------------------------
    # Configuration for selecting atom data structure pointer memory 
    #-------------------------------------------------------
    if (optimized_atom):
      ALIASED_LOCAL_MEMORY_SECTION 			= '.dataL1DSRAM'				#L1 SRAM
    else:
      ALIASED_LOCAL_MEMORY_SECTION 			= '.data'					#L2 SRAM


    #-------------------------------------------------------
    # Configuration for selecting atom data structure memory
    #-------------------------------------------------------
    if (optimized_atom):
      SPECIFIC_BUFMEM_MEMORY_SECTION			= '.gem%d_dataL1DSRAM' % int(coreid)		#L1 SRAM, specific section name obtained by prefix .gem<n>_, 
    else:
      SPECIFIC_BUFMEM_MEMORY_SECTION			= '.gem%d_data' % int(coreid)			#L2 SRAM, specific section name obtained by prefix .gem<n>_, 
    #-------------------------------------------------------

    code_c = ''

    #code_c += '#pragma DATA_SECTION(%s, ".shared_mem");\n' % (instancename, int(coreid))
    #code_c += '#pragma DATA_SECTION(%s, ".shared_mem");\n' % (instancename)
    code_c += '#pragma DATA_SECTION(%s, "%s");\n' % (instancename, ALIASED_LOCAL_MEMORY_SECTION)
    code_c += '#pragma DATA_SECTION(%s_state, "%s");\n' % (instancename, SPECIFIC_BUFMEM_MEMORY_SECTION)
    #code_c += '#pragma DATA_SECTION(%s_mem, ".gem%d_data");\n' % (fifo_name, int(coreid))
    #code_c += '#pragma DATA_ALIGN(%s_mem, 8);\n' % fifo_name

    #code_c += '%s *%s;\n' % (typename, instancename)
    code_c += '%s far *%s;\n' % (typename, instancename)
    code_c += '%s far %s_state;\n' % (typename, instancename)
    #fifo_total_size_in_bytes = int(fifo_n_buffers) * int(fifo_buffer_size_in_bytes)
    #code_c += 'Uint8 %s_mem[%s];\n' % (fifo_name, fifo_total_size_in_bytes)
    #code_c += 'FIFO_BufferState %s_bufferStates[%s];\n' % (fifo_name, fifo_n_buffers)
    code_c += '\n'

    return code_c

  def code_c_atoms_setupRefs(self, instancename):
    code_c = ''
    code_c += '  %s = &%s_state;\n' % (instancename, instancename)
    #code_c += '  CACHE_wbInvAllL1d(CACHE_WAIT);\n'	#touching shared memory
    #code_c += '  CACHE_wbAllL1d(CACHE_WAIT);\n'	#touching shared memory
    #code_c += '  CACHE_wbAllL2(CACHE_WAIT);\n'	#touching shared memory
    return code_c

  def code_c_atoms_setupParams(self, typename, instancename, pars_list):
    code_c = ''

    code_c += '  %s_setupParams(%s' % (typename, instancename)
    for par in pars_list:
      code_c += ', %s' % par
    code_c += ');\n'
    return code_c

  def code_c_atoms_setupWiring(self, typename, instancename, fifo_list):
    code_c = ''

    code_c += '  %s_setupWiring(%s' % (typename, instancename)
    for fifoname in fifo_list:
      if fifoname == "null":
        continue
      #code_c += ', %s' % fifoname
      code_c += ', %s, %s_nBuffersInFifo, %s_bufferStates' % (fifoname, fifoname, fifoname)
    code_c += ');\n'
    return code_c




  def gencode_atom_debug_setup(self, atom_profiling_table, atoms_to_debug, atoms_to_identify, GEN_BLOCK_DEBUG):
    allcode_h_setupAtomDebug = 'void setupAtomDebug();\n'
    allcode_c_setupAtomDebug = 'void setupAtomDebug() {\n'
    self.logger.info('gencode_atom_debug_setup')
    self.logger.info(atom_profiling_table)
    if GEN_BLOCK_DEBUG:
      for r in atom_profiling_table.split('\n')[:-1]:
	atomid, wblockname, atomname = r.split(',')
	if atomname == 'dummy':
	  continue
	atomid = int(atomid)
	to_debug = 0
	if atomname in atoms_to_debug:
	  to_debug = 1
	to_identify = 0
	if atomname in atoms_to_identify:
	  to_identify = 1
	self.logger.debug('%s, %s' % (atomid, atomname))
	allcode_c_setupAtomDebug += '  %s_setupDebug(%s, %d, %d, %d);\n' % (wblockname, atomname, atomid, to_debug, to_identify)
    allcode_c_setupAtomDebug += '}'
    self.logger.debug(allcode_c_setupAtomDebug)
    return allcode_h_setupAtomDebug, allcode_c_setupAtomDebug

  ############################################################################################



  #---------------------
  def gencode_atoms(self, dbo):
  #---------------------
    #<command==atom>, <typename>, <instancename>, <core id>
    #<command==wire>, <instancename>, <fifo1>, [fifo2], ...

    allcode_h = ''
    allcode_h_include = ''
    allcode_h_decl = ''
    allcode_c = ''
    allcode_c_declaration = ''

    allcode_c_setupRefs = 'void setupAtomPtrs() {\n'
    allcode_c_setupWiring = 'void setupAtomWiring() {\n'
    allcode_c_setupParams = 'void setupAtomParams() {\n'
    allcode_c_setup = ''

    db_atoms = dbo.db_select_atoms()
    for row in db_atoms:
      blockname = row["typename"]
      wblockname = row["wblockname"]
      optimized_atom = row["optimized_atom"]

      self.logger.debug('code_h_atoms_include:')
      code = self.code_h_atoms_include(wblockname)
      self.logger.debug(code)
      allcode_h_include += code

      self.logger.debug('code_h_atoms_declaration:')
      code = self.code_h_atoms_declaration(wblockname, row["instancename"])
      self.logger.debug(code)
      allcode_h_decl += code

      self.logger.debug('code_c_atoms_declaration:')
      code = self.code_c_atoms_declaration(wblockname, row["instancename"], row["coreid"], optimized_atom)
      self.logger.debug(code)
      allcode_c_declaration += code

      self.logger.debug('code_c_atoms_setupRefs:')
      code = self.code_c_atoms_setupRefs(row["instancename"])
      self.logger.debug(code)
      allcode_c_setupRefs += code
    allcode_c_setupRefs += '}\n\n'

    #report error if any atom has wiring missing for it
    db_missingwires = dbo.db_select_missingwires()
    if (len(db_missingwires) > 0):
      print_line()
    for row in db_missingwires:
      atomname = row["instancename"]
      print_line()
      self.logger.error("Atom %s is missing wiring" % atomname)
      print_line()
    if (len(db_missingwires) > 0):
      print_line()
      exit(8)



    db_wires = dbo.db_select_wires_with_wblocknames()
    for row in db_wires:
      #atomtypename = row["typename"]
      atomtypename = row["wblockname"]

      self.logger.debug('code_c_atoms_setupWiring:')
      fifolist = row["fifolist"].split(",")
      code = self.code_c_atoms_setupWiring(atomtypename, row["instancename"], fifolist)
      self.logger.debug(code)
      allcode_c_setupWiring += code
    allcode_c_setupWiring += '}\n\n'


    #db_paras = dbo.db_select_paras()
    #for row in db_paras:
    #  atomtypename = row["typename"]

    #  parslist = row["parslist"].split(",")
    #  code = code_c_atoms_setupParams(atomtypename, row["instancename"], parslist)
    #  allcode_c_setupParams += code
    allcode_c_setupParams += '}\n\n'


    allcode_c_setup += allcode_c_setupRefs
    allcode_c_setup += allcode_c_setupWiring
    allcode_c_setup += allcode_c_setupParams

    #allcode_c_setup += '}\n'

    allcode_h += allcode_h_include
    allcode_h += '\n'
    allcode_h += allcode_h_decl
    allcode_h += '\n'
    allcode_h += 'void setupAtoms();\n'
    self.logger.debug('allcode_h:')
    self.logger.debug(allcode_h)
    #f1.write(allcode_h)

    allcode_c += allcode_c_declaration
    allcode_c += '\n'
    allcode_c += allcode_c_setup
    self.logger.debug('allcode_c:')
    self.logger.debug(allcode_c)
    #f2.write(allcode_c)

    return (allcode_h, allcode_c)

  def __init__(self, logger):
    self.logger = logger
