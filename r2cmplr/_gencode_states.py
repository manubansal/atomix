'''
Atomix project, _gencode_states.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from _db import *
from _codegen_write_out import *
from _util import *

class StatesCodeGenerator:

  #---------------------
  def gencode_states(self, dto, dbo, wcg, APPDIRNAME, GEN_BLOCK_DEBUG, atom_profiling_table, atomnamemap, atom_code_table):
  #---------------------
    #print 'atom_profiling_table:', atom_profiling_table
    #print 'atomnamemap:', atomnamemap
    #print 'atom_code_table:', atom_code_table
    max_atomnameidx = max(atomnamemap.values())
    #print 'max_atomid = ', max_atomnameidx
    #raw_input()

    atomnameidx = max_atomnameidx + 1

    self.logger.info('gencode_states')
    stateid_table = {} 

    allcode_h = ''
    allcode_c = ''

    db_n_states = dbo.db_select_n_states()
    db_n_cores  = dbo.db_select_n_cores()
    n_states = db_n_states[0][0]
    n_cores  = int(db_n_cores[0][0]) + 1
    self.logger.info('n_states = %d' % n_states)
    self.logger.info('n_cores  = %d' % n_cores)
    allcode_h += '#define N_STATES %d\n' % n_states
    allcode_h += '#define N_CORES  %d\n\n' % n_cores

    allcode_h += 'void setupStateTable();\n'
    allcode_h += 'void setupInitState();\n\n'

    db_states = dbo.db_select_states()
    allcode_h += 'enum {\n'
    for row in db_states:
      self.logger.debug(row)
      statename = row["statename"]
      statenum  = int(row["statenum"]) - 1		#to base our statenum at 0
      code = '  %s = %d,\n' % (statename, statenum)
      allcode_h += code
    allcode_h += '};\n\n'
    self.logger.debug(allcode_h)

    allcode_c += '\n'
    #allcode_c += '#pragma DATA_SECTION(__KK_stateTable__, ".data");\n'
    allcode_c += 'far KK_StateFxnPtr *__KK_stateTable__;\n\n'
    allcode_c += 'far KK_StateFxnPtr __KK_stateTable__full[N_CORES][N_STATES] = {\n'
    code = ''
    for corenum in range(n_cores):
      code += '  {'
      for row in db_states:
	self.logger.debug(row)
	statename = row["statename"]
	statenum  = int(row["statenum"]) - 1
	#stateid_table.append((str(statenum),statename))
	#stateid_table[str(statenum)] = statename
	stateid_table[statenum] = statename
	code += '%s_core%d, ' % (statename, corenum)
      code += '},\n'
    code += '};\n\n'

    allcode_c += code

    code  = 'void setupStateTable() {\n'
    code += '  __KK_stateTable__ = __KK_stateTable__full[DNUM];\n'
    code += '}\n\n'

    allcode_c += code

    if (self.initStateName == ''):
      print_line()
      self.logger.error('No init state name specified')
      print_line()
      exit(7)

    code = '''
  void setupInitState() {
    FIFO_BufferHandle bh;
    FIFO_Handle ff;
    Uint32 initStateNum = %s;
    Uint32 initIterCount = %s;  
    switch (DNUM) {
    case 0:
      ff = ff_kk0;
      bh = FIFO_getNextWriteBuffer(ff_kk0, ff_kk0_nBuffersInFifo, ff_kk0_bufferStates);
      break;
    case 1:
      ff = ff_kk1;
      bh = FIFO_getNextWriteBuffer(ff_kk1, ff_kk1_nBuffersInFifo, ff_kk1_bufferStates);
      break;
    case 2:
      ff = ff_kk2;
      bh = FIFO_getNextWriteBuffer(ff_kk2, ff_kk2_nBuffersInFifo, ff_kk2_bufferStates);
      break;
    case 3:
      ff = ff_kk3;
      bh = FIFO_getNextWriteBuffer(ff_kk3, ff_kk3_nBuffersInFifo, ff_kk3_bufferStates);
      break;
    default:
      printf("Bad core number\\n");
      exit(1);
    }

    ((Uint32 *)(bh->mem))[0] = initStateNum;
    ((Uint32 *)(bh->mem))[1] = initIterCount;

    FIFO_writeDone(ff, bh);
  }
    ''' % (self.initStateName, self.initIterCount)
    allcode_c += code
    allcode_c += '\n'

    #-------------
    # decways
    #-------------
    #code = 'void setupDecways();\n\n'
    #allcode_h += code

    #db_decways = dbo.db_select_decways()
    #code = 'void setupDecways() {\n'
    #for row in db_decways:
    #  atomname  = row["instancename"]
    #  blockname = row["typename"]
    #  waynum    = row["way"]
    #  statename = row["statename"]
    #  code += '  %s_setupDecway(%s, %s, %s);\n' % (blockname, atomname, waynum, statename)
    #code += '}\n\n'

    #allcode_c += code

    #-------------
    # confs
    #-------------
    db_missingconfs = dbo.db_select_missingconfs()
    if (len(db_missingconfs) > 0):
      print_line()
    for row in db_missingconfs:
      atomname = row["instancename"]
      print_line()
      self.logger.error("Atom %s is missing conf" % atomname)
      print_line()
    if (len(db_missingconfs) > 0):
      print_line()
      exit(8)

    code = 'void setupAtomConfs();\n\n'
    allcode_h += code

    db_confs = dbo.db_select_confs()
    code = 'void setupAtomConfs() {\n'
    for row in db_confs:
      atomname  = row["instancename"]
      blockname = row["typename"]
      vals      = row["vals"]
      code += '  %s_i_conf(&(%s->blconf), %s);\n' % (blockname, atomname, vals)
    code += '}\n\n'

    allcode_c += code

    #---------------
    # state programs
    #---------------
    dummy_atomid = atomnamemap['dummy']
    db_stateps = dbo.db_select_stateps()
    for row in db_stateps:
      fully_qualified_atomname = ''

      self.logger.debug(row)
      statename = row["statename"]
      statenum  = row["statenum"] - 1
      corenum   = row["corenum"]
      axnname   = row["axnname"]
      atomseqid = row["atomseqid"]

      self.logger.debug('code_h_state:')
      code = 'void %s_core%d(Uint32 n);\n' % (statename, corenum)
      self.logger.debug(code)
      allcode_h += code

      self.logger.debug('code_c_state:')
      code  = '//statenum=%d, corenum=%d\n' % (statenum, corenum)
      code += 'void %s_core%d(Uint32 n) {\n' % (statename, corenum)
      if dto.whether_timestamp('st',statename,statenum):
	code += '  SYS_TimeStamp_aliased(%dULL);\n' % (statenum)
      code += '  %s(n);\n' % axnname
      self.logger.debug(code)
      allcode_c += code

      db_atomseq = dbo.db_select_state_atomseq(atomseqid)
      for row in db_atomseq:
	self.logger.debug(row)
	atomname = row["instancename"]
	wblockname = row["wblockname"]

	fully_qualified_atomname = '%s.%s' % (statename, atomname)

	#moving atom id code generation outside the conditional 
	#because atoms can and should always be identified. whether
	#they are timestamped or not is conditional.
	if atomname in atomnamemap:
	  atomid = atomnamemap[atomname]
	else:
	  atomid = atomnameidx
	  atomnamemap[atomname] = atomid
	  atom_profiling_row = []
	  atom_profiling_row.append(str(atomid))	
	  atom_profiling_row.append('%s,%s' % (wblockname,atomname))
	  atom_profiling_table.append(atom_profiling_row)
	  atomnameidx += 1

	if dto.whether_timestamp('at',fully_qualified_atomname):
	  if dto.TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN']:
	    pass
	  else:
	    #if atomname in atomnamemap:
	    #  atomid = atomnamemap[atomname]
	    #else:
	    #  atomid = atomnameidx
	    #  atomnamemap[atomname] = atomid
	    #  atom_profiling_row = []
	    #  atom_profiling_row.append(str(atomid))	
	    #  atom_profiling_row.append('%s,%s' % (wblockname,atomname))
	    #  atom_profiling_table.append(atom_profiling_row)
	    #  atomnameidx += 1
	    code = '      SYS_TimeStamp_aliased(%dULL);\n' % (atomid)
	    allcode_c += code

	if dto.whether_timestamp('atii',atomname):
	  if dto.whether_timestamp('ati'):
	    generated_wblockname = wcg.gencode_wblockTS(dbo, APPDIRNAME, \
		  wblockname, GEN_BLOCK_DEBUG, atomid, False)
	  else:
	    generated_wblockname = wcg.gencode_wblockTS(dbo, APPDIRNAME, \
		  wblockname, GEN_BLOCK_DEBUG, atomid, True)

	  code = '  %s_TS_do(%s);\n' % (wblockname, atomname)
	else:
	  code = '  %s_do(%s);\n' % (wblockname, atomname)

	if dto.whether_timestamp('atii'):
	  if not atomname in atom_code_table:
	    atom_code_table[atomname] = [generated_wblockname + '_do', \
	      blockname + '_i']
	  else:
	    atom_code_table[atomname].append(generated_wblockname + '_do')
	    atom_code_table[atomname].append(blockname + '_i')


	#code = '  %s_do(%s);\n' % (wblockname, atomname)
	allcode_c += code

      #if dto.whether_timestamp('at',fully_qualified_atomname):
      if dto.whether_timestamp('at',""):
	if dto.TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN']:
	  pass
	else:
	  code =  '    //(dummy marker) atomid: %s\n' % dummy_atomid
	  code += '    SYS_TimeStamp_aliased(%dULL);\n' % (dummy_atomid)
	  allcode_c += code

      if dto.whether_timestamp('st',statename,statenum):
	code = '  SYS_TimeStamp_aliased(%dULL);\n' % (statenum)
	allcode_c += code

      allcode_c += '}\n'
      allcode_c += '\n'

    allcode_h += '\n'

    self.logger.debug('allcode_h:')
    self.logger.debug(allcode_h)

    self.logger.debug('allcode_c:')
    self.logger.debug(allcode_c)


    state_profiling_table = stateid_table

    return (allcode_h, allcode_c),state_profiling_table, atom_profiling_table

  def __init__(self, logger):
    self.logger = logger

  def set_init_state(self, initStateName, initIterCount = 1):
    self.initStateName = initStateName
    self.initIterCount = initIterCount
