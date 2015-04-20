/**
Atomix project, OSL_debug.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef OSL_DEBUG_H_
#define OSL_DEBUG_H_

#include <osl/inc/swpform.h>


static inline
void OSL_dumpBuffer(
	Uint32 atomId,
	Uint32 debug,
	char *bufId,
	void *buffer,
	Uint32  nBytes
	){
	
  if (atomId > 0 && debug == 1) {
    //char fname[40];
    //sprintf(fname, "C:\\tmp\\ortrace\\a%d%s.dat", atomId, bufId);
    char fname[sizeof(TRACE_DIR_MIXED) + 40];
    //#error APPDIRNAME_MIXED not definedsprintf(fname, "%s\\a%d%s.dat", TRACE_DIR_MIXED, atomId, bufId);
#ifndef TRACE_DIR_MIXED
#error TRACE_DIR_MIXED not defined
#endif
    sprintf(fname, "%s/__a%d%s.dat", TRACE_DIR_MIXED, atomId, bufId);
    DEBUG_VERBOSE(
    printf("Writing to %s, %d bytes, atomId=%d, bufId=%s\n", fname, nBytes, atomId, bufId);
    )
    FILE *f = fopen(fname, "ab");
    fwrite(buffer, sizeof(char), nBytes, f);
    fclose(f);
  }
}

void OSL_printAtomId(
	Uint32 atomId,
	Uint32 identify
	);

#endif

