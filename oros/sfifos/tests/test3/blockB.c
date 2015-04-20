/**
Atomix project, ./test3/blockB.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>

//This simulates entity B, the data consumer on core 2
void doB(
	FIFO_BufferHandle bh_data_in,
	Uint32 *log_sumB,			//log
	Uint32 log_nextIdx
	) {
  int i = 0;
  int sum = 0;
  //static int iterNo = 0;
  for (i = 0; i < bh_data_in->lengthInBytes/4; i++) {
    sum += ((Uint32 *)bh_data_in->mem)[i];
  }
  //sumB[iterNo++] = sum;
  //printf("B: Read   buffer seq no. %2d\n", ((Uint32 *)bh_data_in->mem)[0]);
  printf("B: Read   buffer seq no. %2d\n", bh_data_in->seqNo);
  printf("B: Buffer sum            %d\n", sum);
  log_sumB[log_nextIdx] = sum;
}

