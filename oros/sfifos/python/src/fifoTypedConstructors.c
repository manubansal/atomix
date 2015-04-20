
//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.

#include <osl/inc/swpform.h>
#include <oros/common/src/sfifos/fifoFactory.h>

#include <oros/common/src/sfifos/python/inc/fifoBufferTypes.h>

FIFO_Status FIFO_createFifo_BUF_Cplx32(FIFO_Handle handle) {
  if (handle == 0)
    return FIFO_FAILURE;
  else
    handle->bufferSizeInBytes = sizeof(BUF_Cplx32);

  return FIFO_createFifo(handle);
}


FIFO_Status FIFO_createFifo_BUF_Funny(FIFO_Handle handle) {
  if (handle == 0)
    return FIFO_FAILURE;
  else
    handle->bufferSizeInBytes = sizeof(BUF_Funny);

  return FIFO_createFifo(handle);
}


