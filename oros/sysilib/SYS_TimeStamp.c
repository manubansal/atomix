/**
Atomix project, SYS_TimeStamp.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <stdio.h>
#include <osl/inc/swpform.h>
#include <ti/csl/csl_tsc.h>
#include "SYS_TimeStamp.h"

#ifdef SYS_LOG_TO_FILE
#define MYLOG(...) fprintf(fid, __VA_ARGS__)
#else
#define MYLOG(...) printf(__VA_ARGS__)
#endif


//#define N_TS 1000	//good for 10 wrap arounds of 10000 sample trace (max idx = 1500 or so)
//#define N_TS 2000
//#define N_TS 4000
#define N_TS 10000
//#define N_TS 8000
////#define N_TS 12000
#define N_TS_INTO_2	(N_TS * 2)

#define DEBUG_TS

far Uint64 SYS_TS_aliased[N_TS * 2];
static far Uint32 idx_aliased = 0;


void SYS_TimeStamp_aliased(Uint64 id) {

#ifdef DEBUG_TS
  if (idx_aliased + 2 >= N_TS_INTO_2) {
    printf("ERROR: TS buffer will overflow.\n");
    SYS_TimeStampPrintAllAliased();
    exit(1);
  }
#endif

  SYS_TS_aliased[idx_aliased++] = id;
  SYS_TS_aliased[idx_aliased++] = CSL_tscRead();
}

void SYS_TimeStampPrintAllAliased() {
  Uint32 i;

  printf("Writing syslog to file: %s\n", SYS_LOG_FILE);
#ifdef SYS_LOG_TO_FILE
  FILE *fid = fopen(SYS_LOG_FILE, "w");
#endif

  MYLOG("Printing all timestamps\n");
  MYLOG("Last timestamp idx written: %d, %d, %d, %d\n", 
  	*(Uint32 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)&idx_aliased, 0), 
  	*(Uint32 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)&idx_aliased, 1), 
  	*(Uint32 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)&idx_aliased, 2), 
  	*(Uint32 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)&idx_aliased, 3)); 

  Uint32 coreId = 0;
  for (coreId = 0; coreId < 4; coreId++) {
    for (i = 0; i < *(Uint32 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)&idx_aliased, coreId); i+=2) {
      MYLOG("DNUM %d  id  %llu    ts  %llu\n", 
      	coreId,
  	((Uint64 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)SYS_TS_aliased, coreId))[i], 
  	((Uint64 *)LOCAL_TO_GLOBAL_ADDRESS((Uint32)SYS_TS_aliased, coreId))[i + 1]);
    }
  }

#ifdef SYS_LOG_TO_FILE
  fclose(fid);
#endif
  printf("Written syslog to file: %s\n", SYS_LOG_FILE);

  exit(0);

}
//----------------------------------------------------
