/**
Atomix project, ./test_s2/old/setupFifo.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <c6x.h>
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include <ipc/ipc_cpy.h>

void setupFifo(
    FIFO_Handle ff,
    FIFO_State *ff_state,
    Uint8 ff_mem[],
    FIFO_BufferState *ff_bufferStates[],
    Uint32 ff_BUFFER_SIZE_IN_BYTES,
    Uint32 ff_N_BUFFERS,
    FIFO_TYPE_SRC_OR_DST ff_type
    );
