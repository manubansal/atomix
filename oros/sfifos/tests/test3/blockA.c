/**
Atomix project, ./test3/blockA.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <c6x.h>
#include <ti/csl/csl_tsc.h>

//This simulates entity A, the data producer on core 1
void doA(
	FIFO_BufferHandle bh_data_out, 	//out
	Uint32 *log_sumA,			//log
	Uint32 log_nextIdx
	) {
  int i = 0;
  int sum = 0;
  int r = _loll(CSL_tscRead()) & 0x00FF;
  for (i = 0; i < bh_data_out->lengthInBytes/4; i++) {
    ((Uint32 *)bh_data_out->mem)[i] = r * (i + 1);	//fill data
    sum += ((Uint32 *)bh_data_out->mem)[i];
  }
  //sumA[iterNo++] = sum;
  //printf("A: Filled buffer seq no. %2d\n", ((Uint32 *)bh_data_out->mem)[0]);
  printf("A: Filled buffer seq no. %2d\n", bh_data_out->seqNo); 
  printf("A: Buffer sum            %d\n", sum);
  log_sumA[log_nextIdx] = sum;
}
