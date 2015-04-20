'''
Atomix project, _gencode_wblock.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from _db import *
from _codegen_write_out import *
from _util import *

class WblockCodeGenerator:

  #---------------------
  def gencode_wblock(self, wblockrow, GEN_BLOCK_DEBUG=False, GEN_DO_FUNC = True, TS=False, ii_only=False, atomid=0):
  #---------------------
    (wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string) = wblockrow

    allcode_h = ''
    allcode_c = ''

    inp_list = inptstr.split(',') if inptstr != '' else []
    out_list = outtstr.split(',') if outtstr != '' else []
    cf_list = conftstr.split(',') if conftstr != '' else []
    fifo_access_signature = fifo_access_signature_string.split(',')
    inp_signature = fifo_access_signature[0:len(inp_list)] if inptstr != '' else []
    out_signature = fifo_access_signature[len(inp_list):(len(inp_list) + len(out_list))] if outtstr != '' else []
    self.logger.debug(wblockname)
    self.logger.debug('%s, %s' % (inp_list, inp_signature))
    self.logger.debug('%s, %s' % (out_list, out_signature))

    code_blocktype = 'typedef struct {\n'
    code_h_setupWiring  = 'void %s_setupWiring (\n' % wblockname
    code_h_setupWiring += '  %s *bli' % wblockname
    code_h_setupDebug   = 'void %s_setupDebug (\n' % wblockname
    code_h_setupDebug  += '  %s *bli' % wblockname
    code_h_setupDebug      += ',\n  Uint32 atomid'
    code_h_setupDebug      += ',\n  Uint32 debug'
    code_h_setupDebug      += ',\n  Uint32 identify'
    if TS:
      ##code_h_do           = 'void %s_TS%d_do (%s *bli);\n' % (wblockname, atomid, wblockname)
      code_h_do           = 'void %s_TS_do (%s *bli);\n' % (wblockname, wblockname)
    else:
      if GEN_DO_FUNC:
	code_h_do           = 'void %s_do (%s *bli);\n' % (wblockname, wblockname)
      else:
	code_h_do		  = ''

    code_c_setupWiring  = code_h_setupWiring
    code_c_setupDebug   = code_h_setupDebug
    if TS:
      ##code_c_do           = 'void %s_TS%d_do (%s *bli) {\n' % (wblockname, atomid, wblockname)
      code_c_do           = 'void %s_TS_do (%s *bli) {\n' % (wblockname, wblockname)
    else:
      if GEN_DO_FUNC:
	code_c_do           = 'void %s_do (%s *bli) {\n' % (wblockname, wblockname)
      else:
	code_c_do		  = ''
    code_c_do_decl      = '';
    code_c_do_cmd1      = '';
    code_c_do_cmd1d     = '';
    code_c_do_cmd2      = '';
    code_c_do_cmd2d     = '';
    code_c_do_cmd3      = '';

    code_c_setupWiring_body = ''
    code_c_setupDebug_body  = ''

    atomid_string = 'bli->atomid'
    to_identify_string = 'bli->identify'
    if GEN_BLOCK_DEBUG:
      code_c_do += '  OSL_printAtomId(%s, %s);\n' % (atomid_string, to_identify_string)

    for i in range(len(inp_list)):
      code_blocktype   += '  FIFO_Handle ff_inp%d;\n' % i
      code_blocktype   += '  Uint32 ff_inp%d_nBuffersInFifo;\n' % i
      code_blocktype   += '  FIFO_BufferHandle ff_inp%d_bufferStates;\n' % i

      code_h_setupWiring += ',\n  FIFO_Handle ff_inp%d' % i
      code_h_setupWiring += ',\n  Uint32 ff_inp%d_nBuffersInFifo' % i
      code_h_setupWiring += ',\n  FIFO_BufferHandle ff_inp%d_bufferStates' % i

      code_c_setupWiring += ',\n  FIFO_Handle ff_inp%d' % i
      code_c_setupWiring += ',\n  Uint32 ff_inp%d_nBuffersInFifo' % i
      code_c_setupWiring += ',\n  FIFO_BufferHandle ff_inp%d_bufferStates' % i

      code_c_setupWiring_body += '    bli->ff_inp%d = ff_inp%d;\n' % (i, i)
      code_c_setupWiring_body += '    bli->ff_inp%d_nBuffersInFifo = ff_inp%d_nBuffersInFifo;\n' % (i, i)
      code_c_setupWiring_body += '    bli->ff_inp%d_bufferStates = ff_inp%d_bufferStates;\n' % (i, i)

      code_c_do_decl     += '  FIFO_BufferHandle bhi%d;\n' % i
      if (inp_signature[i] == 'r'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextReadBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'u'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextReReadBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'o'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextReReIcBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 't' or inp_signature[i] == 's'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextReadBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'e'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReadBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == '1'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReIcBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'i'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReadBusyBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == '5'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReIcBusyBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == '9'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReIcBusyBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'v'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextReReadBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'p'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextReReIcBuffer_remote(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'd'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReadBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == '0'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReIcBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == 'h'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReadBusyBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == '4'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReIcBusyBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      elif (inp_signature[i] == '8'):
	code_c_do_cmd1     += '  bhi%d = FIFO_getNextAsyncReIcBusyBuffer(bli->ff_inp%d, bli->ff_inp%d_nBuffersInFifo, bli->ff_inp%d_bufferStates);\n' % (i, i, i, i)
      else:
	print_line()
	self.logger.error("I do not know how to generate code for inp_signature %s" % inp_signature[i])
	print_line()
	sys.exit(1)

      atomid = 'bli->atomid'
      to_debug = 'bli->debug'
      bufid = 'bhi%d' % i
      bufmem = bufid + '->mem'

      if inp_list[i] == 'void':
	nbytes = 'bli->ff_inp%d->bufferSizeInBytes' % i
      else:
	nbytes = 'sizeof(%s)' % inp_list[i]
      code_c_do_cmd1d	+= '  OSL_dumpBuffer(%s, %s, "%s", %s, %s);\n' % (atomid, to_debug, bufid, bufmem, nbytes)

      code_c_do_cmd2     += '    (%s *)(bhi%d->mem)' % (inp_list[i], i)
      if (i < len(inp_list) + len(out_list) - 1):
	code_c_do_cmd2 += ','
      code_c_do_cmd2     += '\n'
      if (inp_signature[i] == 'r'):
	code_c_do_cmd3     += '  FIFO_readDone(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 'u'):
	code_c_do_cmd3     += '  FIFO_reReadDone(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 'o'):
	code_c_do_cmd3     += '  FIFO_reReadDone(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 't' or inp_signature[i] == 's'):
	code_c_do_cmd3     += '  FIFO_readDone_remote(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 'e'):
	pass
      elif (inp_signature[i] == '1'):
	pass
      elif (inp_signature[i] == 'i'):
	code_c_do_cmd3     += '  FIFO_readDone_remote(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == '5'):
	code_c_do_cmd3     += '  FIFO_readDone_remote(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == '9'):
	code_c_do_cmd3     += '  FIFO_reReadDone_remote(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 'v'):
	code_c_do_cmd3     += '  FIFO_reReadDone_remote(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 'p'):
	code_c_do_cmd3     += '  FIFO_reReadDone_remote(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == 'd'):
	pass		#no buffer return code in this case
      elif (inp_signature[i] == '0'):
	pass		#no buffer return code in this case
      elif (inp_signature[i] == 'h'):
	code_c_do_cmd3     += '  FIFO_readDone(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == '4'):
	code_c_do_cmd3     += '  FIFO_readDone(bli->ff_inp%d, bhi%d);\n' % (i, i)
      elif (inp_signature[i] == '8'):
	code_c_do_cmd3     += '  FIFO_reReadDone(bli->ff_inp%d, bhi%d);\n' % (i, i)
      else:
	print_line()
	self.logger.error("I do not know how to generate code for inp_signature %s" % inp_signature[i])
	print_line()
	sys.exit(1)

    for i in range(len(out_list)):
      code_blocktype += '  FIFO_Handle ff_out%d;\n' % i
      code_blocktype += '  Uint32 ff_out%d_nBuffersInFifo;\n' % i
      code_blocktype += '  FIFO_BufferHandle ff_out%d_bufferStates;\n' % i

      code_h_setupWiring += ',\n  FIFO_Handle ff_out%d' % i
      code_h_setupWiring += ',\n  Uint32 ff_out%d_nBuffersInFifo' % i
      code_h_setupWiring += ',\n  FIFO_BufferHandle ff_out%d_bufferStates' % i

      code_c_setupWiring += ',\n  FIFO_Handle ff_out%d' % i
      code_c_setupWiring += ',\n  Uint32 ff_out%d_nBuffersInFifo' % i
      code_c_setupWiring += ',\n  FIFO_BufferHandle ff_out%d_bufferStates' % i

      code_c_setupWiring_body += '    bli->ff_out%d = ff_out%d;\n' % (i, i)
      code_c_setupWiring_body += '    bli->ff_out%d_nBuffersInFifo = ff_out%d_nBuffersInFifo;\n' % (i, i)
      code_c_setupWiring_body += '    bli->ff_out%d_bufferStates = ff_out%d_bufferStates;\n' % (i, i)

      code_c_do_decl     += '  FIFO_BufferHandle bho%d;\n' % i
      if (out_signature[i] == 'y' or out_signature[i] == 'x'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextWriteBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'n'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextReWriteBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'z'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextReWrIcBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'g'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriteBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == '3'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriIcBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'l'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriteBusyBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == '7'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriIcBusyBuffer_remote(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'f'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriteBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == '2'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriIcBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'k'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriteBusyBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == '6'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextAsyncWriIcBusyBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'w'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextWriteBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'm'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextReWriteBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      elif (out_signature[i] == 'q'):
	code_c_do_cmd1     += '  bho%d = FIFO_getNextReWrIcBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      #else:
      #  code_c_do_cmd1     += '  bho%d = FIFO_getNextWriteBuffer(bli->ff_out%d, bli->ff_out%d_nBuffersInFifo, bli->ff_out%d_bufferStates);\n' % (i, i, i, i)
      else:
	print_line()
	self.logger.error("I do not know how to generate code for out_signature %s" % out_signature[i])
	print_line()
	sys.exit(1)

      code_c_do_cmd2     += '    (%s *)(bho%d->mem)' % (out_list[i], i)
      if (i < len(out_list) - 1):
	code_c_do_cmd2 += ','
	code_c_do_cmd2 += '\n'

      if (out_signature[i] == 'y' or out_signature[i] == 'x'):
	code_c_do_cmd3     += '  FIFO_writeDone_remote(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'n'):
	code_c_do_cmd3     += '  FIFO_reWriteDone_remote(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'z'):
	code_c_do_cmd3     += '  FIFO_reWriteDone_remote(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'g'):
	pass
      elif (out_signature[i] == '3'):
	pass
      elif (out_signature[i] == 'l'):
	code_c_do_cmd3     += '  FIFO_writeDone_remote(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == '7'):
	code_c_do_cmd3     += '  FIFO_writeDone_remote(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'f'):
	pass		#no buffer return code in this case
      elif (out_signature[i] == '2'):
	pass		#no buffer return code in this case
      elif (out_signature[i] == 'k'):
	code_c_do_cmd3     += '  FIFO_writeDone(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == '6'):
	code_c_do_cmd3     += '  FIFO_writeDone(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'w'):
	code_c_do_cmd3     += '  FIFO_writeDone(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'm'):
	code_c_do_cmd3     += '  FIFO_reWriteDone(bli->ff_out%d, bho%d);\n' % (i, i)
      elif (out_signature[i] == 'q'):
	code_c_do_cmd3     += '  FIFO_reWriteDone(bli->ff_out%d, bho%d);\n' % (i, i)
      #else:
      #  code_c_do_cmd3     += '  FIFO_writeDone(bli->ff_out%d, bho%d);\n' % (i, i)
      else:
	print_line()
	self.logger.error("I do not know how to generate code for out_signature %s" % out_signature[i])
	print_line()
	sys.exit(1)

      atomid = 'bli->atomid'
      to_debug = 'bli->debug'
      bufid = 'bho%d' % i
      bufmem = bufid + '->mem'
      if out_list[i] == 'void':
	nbytes = 'bli->ff_out%d->bufferSizeInBytes' % i
      else:
	nbytes = 'sizeof(%s)' % out_list[i]
      code_c_do_cmd2d	+= '  OSL_dumpBuffer(%s, %s, "%s", %s, %s);\n' % (atomid, to_debug, bufid, bufmem, nbytes)

    code_c_setupDebug_body += '  \n  bli->atomid = atomid;\n'
    code_c_setupDebug_body += '  \n  bli->debug = debug;\n'
    code_c_setupDebug_body += '  \n  bli->identify = identify;\n'

    if (len(cf_list) > 1):
      self.logger.error("At most one configuration type (CF) io is allowed")
      exit(1)

    for i in range(len(cf_list)):
      code_blocktype     += '  %s blconf;\n' % (cf_list[i])
      if i == 0 and len(inp_list) + len(out_list) == 0:
        pass
      else:
	code_c_do_cmd2     += ',\n'
      code_c_do_cmd2     += '    &(bli->blconf)'

    code_c_do_cmd2     += '\n'

    if GEN_BLOCK_DEBUG:
      code_blocktype += '  Uint32 atomid;\n'
      code_blocktype += '  Uint32 debug;\n'
      code_blocktype += '  Uint32 identify;\n'

    code_blocktype     += '} %s;\n' % wblockname
    code_h_setupWiring += '\n  );\n'
    code_c_setupWiring += '\n  ){\n'
    code_c_setupWiring += code_c_setupWiring_body
    code_c_setupWiring += '}\n'
    code_h_setupDebug += '\n  );\n'
    code_c_setupDebug += '\n  ){\n'
    code_c_setupDebug += code_c_setupDebug_body
    code_c_setupDebug += '}\n'
    code_c_do          += '%s\n' % code_c_do_decl
    if TS and not ii_only:
      code_c_do 		+= '  SYS_TimeStamp_aliased(%s);\n\n' % (atomid)
    code_c_do          += '%s\n' % code_c_do_cmd1
    if TS:
      code_c_do 		+= '  SYS_TimeStamp_aliased(%s);\n\n' % (atomid)

    if GEN_BLOCK_DEBUG:
      code_c_do  += code_c_do_cmd1d + '\n'

    code_c_do          += '  %s_i(\n' % blockname
    code_c_do          += '%s' % code_c_do_cmd2
    code_c_do          += '  );\n\n'

    if GEN_BLOCK_DEBUG:
      code_c_do  += code_c_do_cmd2d + '\n'

    if TS:
      code_c_do 		+= '  SYS_TimeStamp_aliased(%s);\n\n' % (atomid)
    code_c_do          += '%s\n' % code_c_do_cmd3
    if TS and not ii_only:
      code_c_do 		+= '  SYS_TimeStamp_aliased(%s);\n\n' % (atomid)
    code_c_do          += '}\n'

    if not GEN_BLOCK_DEBUG:
      code_h_setupDebug = '';
      code_c_setupDebug = '';

    self.logger.debug('allcode_h')
    if TS:
      code = '%s\n' % (code_h_do)
    else:
      #code = '%s\n%s\n%s\n' % (code_blocktype, code_h_setupWiring, code_h_do)
      code = '%s\n%s\n%s\n%s\n' % (code_blocktype, code_h_setupWiring, code_h_setupDebug, code_h_do)
    self.logger.debug(code)
    allcode_h = code

    self.logger.debug('allcode_c')
    if TS:
      code = '%s\n' % (code_c_do)
    else:
      if GEN_DO_FUNC:
	#code = '%s\n%s\n' % (code_c_setupWiring, code_c_do)
	code = '%s\n%s\n%s\n' % (code_c_setupWiring, code_c_setupDebug, code_c_do)
      else:
	#code = '%s\n' % (code_c_setupWiring)
	code = '%s\n%s\n' % (code_c_setupWiring, code_c_setupDebug)
    self.logger.debug(code)
    allcode_c = code
      
    return (allcode_h, allcode_c)


  ###########################
  def gencode_wblocks(self, dbo, APPDIRNAME, GEN_BLOCK_DEBUG, GEN_DO_FUNC = True, atoms_already_generated=[]):
    wblockdir = APPDIRNAME
    db_wblocks = dbo.db_select_distinct_wblocks()
    for row in db_wblocks:
      wblockname = row['wblockname']
      blockname = row['blockname']
      wblock_code_h, wblock_code_c = self.gencode_wblock(row, GEN_BLOCK_DEBUG, GEN_DO_FUNC)
      wblock_h_file = wblockdir + '/__' + wblockname + '__.h'
      wblock_c_file = wblockdir + '/__' + wblockname + '__.c'
      codegen_write_out_wblock(wblockname, blockname, wblock_h_file, wblock_c_file, wblock_code_h, wblock_code_c, GEN_BLOCK_DEBUG)

  ###########################
  def gencode_wblockTS(self, dbo, APPDIRNAME, wblockname, GEN_BLOCK_DEBUG, atomid, ii_only=False):
    wblockdir = APPDIRNAME

    db_wblocks = dbo.db_select_wblock_row(wblockname)
    row = db_wblocks[0]
    wblockname = row['wblockname']
    blockname = row['blockname']
    wblock_code_h, wblock_code_c = self.gencode_wblock(row, GEN_BLOCK_DEBUG, True, True, ii_only, atomid)

    owblockname = wblockname
    ##wblockname = '%s_TS%d' % (wblockname, atomid)
    wblockname = '%s_TS' % (wblockname)

    wblock_h_file = wblockdir + '/__' + wblockname + '__.h'
    wblock_c_file = wblockdir + '/__' + wblockname + '__.c'
    codegen_write_out_wblock(wblockname, blockname, wblock_h_file, wblock_c_file, wblock_code_h, wblock_code_c, GEN_BLOCK_DEBUG, True, owblockname)

    generated_wblockname = wblockname
    return generated_wblockname


  def __init__(self, logger):
    self.logger = logger
