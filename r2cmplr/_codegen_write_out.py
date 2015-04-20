'''
Atomix project, _codegen_write_out.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

def codegen_write_out_wblock(
	wblockname, \
	blockname, \
  	wblock_h_file, \
	wblock_c_file, \
	wblock_allcode_h, \
	wblock_allcode_c, \
	GEN_BLOCK_DEBUG=False,
	TS=False,
	owblockname='jj'
	):
  #wblock_h_CanonicalName = "____FIFOS_H____"
  wblock_h_CanonicalName = "__%s_H__" % wblock_h_file.split("/")[-1].split(".h")[0].upper()
  wblock_h_BaseName = wblock_h_file.split("/")[-1]

  wblock_h_ = 0
  wblock_c_ = 0



  #----------------------------

  commonHeader = """
//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>

""" 
  if GEN_BLOCK_DEBUG:
    commonHeader += "#include <osl/OSL_debug.h>\n"

  if TS:
    commonHeader += '#include "__%s__.h"\n' % (owblockname)

  wblock_h_header = """
#ifndef """ + wblock_h_CanonicalName + """
#define """ + wblock_h_CanonicalName + """

""" + commonHeader + """
#include "%s_t.h"
""" % (blockname)


  wblock_h_tailer = """
#endif //""" + wblock_h_CanonicalName + """
"""

#include <oros/sfifos/python/inc/fifoBufferTypes.h>

  wblock_c_header = commonHeader + """
#include "%s_t.h"
#include "%s_i.h"
#include "%s"

""" % (blockname, blockname, wblock_h_BaseName)

  if TS:
    wblock_c_header	+= '#include <oros/sysilib/SYS_TimeStamp.h>\n'

  wblock_c_tailer = """
"""

  wblock_h_ = open(wblock_h_file,'w')
  wblock_h_.write(wblock_h_header)

  wblock_c_ = open(wblock_c_file, 'w')
  wblock_c_.write(wblock_c_header)

  wblock_h_.write(wblock_allcode_h)
  wblock_c_.write(wblock_allcode_c)

  wblock_h_.write(wblock_h_tailer)
  wblock_h_.close()

  wblock_c_.write(wblock_c_tailer)
  wblock_c_.close()

  return wblock_h_BaseName



def codegen_write_out_fifos(\
  	atoms_h_BaseName, \
  	fifos_h_file, \
	fifos_c_file, \
	fifos_allcode_h, \
	fifos_allcode_c \
	):

  #fifos_h_CanonicalName = "____FIFOS_H____"
  fifos_h_CanonicalName = "__%s_H__" % fifos_h_file.split("/")[-1].split(".h")[0].upper()
  fifos_h_BaseName = fifos_h_file.split("/")[-1]

  fifos_h_ = 0
  fifos_c_ = 0



  #----------------------------

  commonHeader = """
//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.

#include <osl/inc/swpform.h>
#include "setup.h"
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include "%s" 

""" % atoms_h_BaseName

  fifos_h_header = """
#ifndef """ + fifos_h_CanonicalName + """
#define """ + fifos_h_CanonicalName + """

""" + commonHeader + """
"""

  fifos_h_tailer = """
#endif //""" + fifos_h_CanonicalName + """
"""

#include <oros/sfifos/python/inc/fifoBufferTypes.h>

  fifos_c_header = commonHeader + """
#include <c6x.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>

"""

  fifos_c_tailer = """
"""

  fifos_h_ = open(fifos_h_file,'w')
  fifos_h_.write(fifos_h_header)

  fifos_c_ = open(fifos_c_file, 'w')
  fifos_c_.write(fifos_c_header)

  fifos_h_.write(fifos_allcode_h)
  fifos_c_.write(fifos_allcode_c)

  fifos_h_.write(fifos_h_tailer)
  fifos_h_.close()

  fifos_c_.write(fifos_c_tailer)
  fifos_c_.close()

  return fifos_h_BaseName


def codegen_write_out_atoms(\
	fifos_h_BaseName,
	atoms_h_file, \
	atoms_c_file, \
	atoms_allcode_h, \
	atoms_allcode_c \
	):

  #atoms_h_CanonicalName = "____ATOMS_H____"
  atoms_h_CanonicalName = "__%s_H__" % atoms_h_file.split("/")[-1].split(".h")[0].upper()
  atoms_h_BaseName = atoms_h_file.split("/")[-1]

  atoms_h_ = 0
  atoms_c_ = 0

  #----------------------------
  commonHeader = """
//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.

#include <osl/inc/swpform.h>
#include "setup.h"
"""

  atoms_h_header = """
#ifndef """ + atoms_h_CanonicalName + """
#define """ + atoms_h_CanonicalName + """

""" + commonHeader + """
"""

  atoms_h_tailer = """
#endif //""" + atoms_h_CanonicalName + """
"""

#include <oros/sfifos/python/inc/fifoBufferTypes.h>

  atoms_c_header = commonHeader + """
""" + "#include \"" + atoms_h_BaseName + "\"\n" + "#include \"" + fifos_h_BaseName + "\"\n\n" + \
"""
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>

"""

  atoms_c_tailer = """
"""

  atoms_h_ = open(atoms_h_file,'w')
  atoms_h_.write(atoms_h_header)

  atoms_c_ = open(atoms_c_file, 'w')
  atoms_c_.write(atoms_c_header)

  atoms_h_.write(atoms_allcode_h)
  atoms_c_.write(atoms_allcode_c)

  atoms_h_.write(atoms_h_tailer)
  atoms_h_.close()

  atoms_c_.write(atoms_c_tailer)
  atoms_c_.close()

  return atoms_h_BaseName

  #----------------------------------------

def codegen_write_out_actions(\
	TIMESTAMP_CONFIG, \
	atoms_h_BaseName,
	actions_h_file, \
	actions_c_file, \
	actions_allcode_h, \
	actions_allcode_c, \
	axn_profiling_table, \
	axn_profiling_table_file, \
	atom_profiling_table, \
	atom_profiling_table_file \
	):

  actions_h_CanonicalName = "__%s_H__" % actions_h_file.split("/")[-1].split(".h")[0].upper()
  actions_h_BaseName = actions_h_file.split("/")[-1]

  actions_h_ = 0
  actions_c_ = 0

  #----------------------------
  commonHeader  	 = "//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.\n\n"
  commonHeader 		+= "#include <osl/inc/swpform.h>\n"
  commonHeader		+= '#include "setup.h"\n'
  actions_h_header  	 = "#ifndef %s\n" % actions_h_CanonicalName
  actions_h_header 	+= "#define %s\n\n" % actions_h_CanonicalName
  actions_h_header 	+= "%s\n" % commonHeader

  actions_h_tailer 	 = "#endif //%s\n" % actions_h_CanonicalName

  actions_c_header  	 = commonHeader + '\n'
  actions_c_header	+= '#include "%s"\n' % atoms_h_BaseName
  actions_c_header 	+= '#include "%s"\n' % actions_h_BaseName 
  if (TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS']):
    actions_c_header	+= '#include <oros/sysilib/SYS_TimeStamp.h>\n'

  actions_c_tailer 	 = ''

  actions_h_ = open(actions_h_file,'w')
  actions_h_.write(actions_h_header)
  actions_c_ = open(actions_c_file, 'w')
  actions_c_.write(actions_c_header)
  actions_h_.write(actions_allcode_h)
  actions_c_.write(actions_allcode_c)
  actions_h_.write(actions_h_tailer)
  actions_h_.close()
  actions_c_.write(actions_c_tailer)
  actions_c_.close()

  axn_profiling_table_f = open(axn_profiling_table_file, 'w')
  axn_profiling_table_f.write(axn_profiling_table)
  axn_profiling_table_f.close() 

  atom_profiling_table_f = open(atom_profiling_table_file, 'w')
  atom_profiling_table_f.write(atom_profiling_table)
  atom_profiling_table_f.close() 

  return actions_h_BaseName


def codegen_write_out_states(\
	TIMESTAMP_CONFIG, \
	actions_h_BaseName, \
	atoms_h_BaseName, \
	fifos_h_BaseName, \
	states_h_file, \
	states_c_file, \
	states_allcode_h, \
	states_allcode_c, \
	state_profiling_table, \
	state_profiling_table_file \
	):

  states_h_CanonicalName = "__%s_H__" % states_h_file.split("/")[-1].split(".h")[0].upper()
  states_h_BaseName = states_h_file.split("/")[-1]

  states_h_ = 0
  states_c_ = 0

  #----------------------------
  commonHeader  	 = "//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.\n\n"
  commonHeader 		+= "#include <osl/inc/swpform.h>\n"
  commonHeader		+= '#include "setup.h"\n'
  commonHeader 		+= "#include <c6x.h>\n"

  states_h_header  	 = "#ifndef %s\n" % states_h_CanonicalName
  states_h_header 	+= "#define %s\n\n" % states_h_CanonicalName
  states_h_header 	+= "%s\n" % commonHeader

  states_h_tailer 	 = "#endif //%s\n" % states_h_CanonicalName

  states_c_header  	 = commonHeader + '\n'
  states_c_header	+= '#include "%s"\n' % atoms_h_BaseName
  states_c_header	+= '#include "%s"\n' % fifos_h_BaseName
  states_c_header	+= '#include "%s"\n' % actions_h_BaseName
  states_c_header 	+= '#include "%s"\n' % states_h_BaseName 
  states_c_header	+= '#include <oros/sysilib/BlockKK_stateTable.h>\n'
  if (TIMESTAMP_CONFIG['TIMESTAMP_STATES']):
    states_c_header	+= '#include <oros/sysilib/SYS_TimeStamp.h>\n'

  states_c_tailer 	 = ''

  states_h_ = open(states_h_file,'w')
  states_h_.write(states_h_header)
  states_c_ = open(states_c_file, 'w')
  states_c_.write(states_c_header)
  states_h_.write(states_allcode_h)
  states_c_.write(states_allcode_c)
  states_h_.write(states_h_tailer)
  states_h_.close()
  states_c_.write(states_c_tailer)
  states_c_.close()

  state_profiling_table_f = open(state_profiling_table_file, 'w')
  state_profiling_table_f.write(state_profiling_table)
  state_profiling_table_f.close() 


