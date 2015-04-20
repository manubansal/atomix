
#ifndef __FIFO_BUFFER_TYPES_H__
#define __FIFO_BUFFER_TYPES_H__


//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.

#include <osl/inc/swpform.h>
#include <oros/common/src/sfifos/fifoFactory.h>


typedef Cplx32 BUF_Cplx32;

typedef struct {
      Uint32 i;
      Uint32 j;
      Uint32 k;
      Uint32 l;
} BUF_Funny;

FIFO_Status FIFO_createFifo_BUF_Cplx32(FIFO_Handle handle);


FIFO_Status FIFO_createFifo_BUF_Funny(FIFO_Handle handle);


#endif //__FIFO_BUFFER_TYPES_H__
