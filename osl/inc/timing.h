/**
Atomix project, timing.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <ti/csl/csl_tsc.h>
#include <stdio.h>


#ifdef ENABLE_TIMING
#define MEASURE_RUN_TIME(func) \
{ \
uint64_t pre,post; \
pre = CSL_tscRead(); \
func \
post = CSL_tscRead(); \
printf(__FUNCTION__); \
printf(" Cycles: %lld \n",post-pre); \
} 
#else
#define MEASURE_RUN_TIME(func) \
func
#endif

