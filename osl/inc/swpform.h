/**
Atomix project, swpform.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef _SWPFORM_H_
#define _SWPFORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "amem4cpy_inl.h"


#include <ti/csl/tistdtypes.h>
//#include "CI_types.h"
#include "or_types.h"
#include <oros/sysilib/SYS_TimeStamp.h>
#include <osl/LOG_logger.h>

#define IN
#define OUT
#define INOUT
#define CF


#ifndef DEBUGLEVEL
#error "DEBUGLEVEL not defined"
#endif

#if DEBUGLEVEL > 0
#define DEBUG_LEVEL_1
#define DEBUG_LEVEL_ERROR
#endif

#if DEBUGLEVEL > 1
#define DEBUG_LEVEL_2
#define DEBUG_LEVEL_INFO
#endif

#if DEBUGLEVEL > 2
#define DEBUG_LEVEL_3
#define DEBUG_LEVEL_DATA
#endif

#if DEBUGLEVEL > 3
#define DEBUG_LEVEL_4
#define DEBUG_LEVEL_INIT
#define DEBUG_LEVEL_VERBOSE
#endif


/****************************************/

#ifdef DEBUG_LEVEL_ERROR
#define DEBUG_ERROR(x)	x
#else
#define DEBUG_ERROR(x)
#endif

#ifdef DEBUG_LEVEL_INFO
#define DEBUG_INFO(x)	x
#else
#define DEBUG_INFO(x)
#endif

#ifdef DEBUG_LEVEL_DATA
#define DEBUG_DATA(x)	x
#else
#define DEBUG_DATA(x)
#endif

#ifdef DEBUG_LEVEL_INIT
#define DEBUG_INIT(x)	x
#else
#define DEBUG_INIT(x)
#endif

#ifdef DEBUG_LEVEL_VERBOSE
#define DEBUG_VERBOSE(x)	x
#else
#define DEBUG_VERBOSE(x)
#endif

#ifdef DEBUG_LEVEL_1
#define DEBUG(x)	x
#else
#define DEBUG(x)
#endif

#ifdef DEBUG_LEVEL_2
#define DEBUG2(x)	x
#else
#define DEBUG2(x)
#endif

#ifdef DEBUG_LEVEL_3
#define DEBUG3(x)	x
#else
#define DEBUG3(x)
#endif

#ifdef DEBUG_LEVEL_3
#define DEBUG_ALL(x)	x
#else
#define DEBUG_ALL(x)
#endif


#define MAXUINT32 0xFFFFFFFF
					//use this for debug output,
					//comment out when profiling

#define ENABLE_VITERBI_VCP2
//#define VCP2_BREAK_ON_EDMA_COMPLETION		//use this on the platform
#define VCP2_BREAK_ON_NUM_SYM_PROC		//use this on the simulator

//#define USE_ASSERT				//use this when debugging,
						//it builds in asserts. if
						//not defined, ASSERT is
						//replaced with blanks.




#define SW_BREAKPOINT asm( " .long 0x1001E000" )


#define PRAGMA(x) _Pragma(#x)

#ifdef USE_ASSERT
//#define ASSERT(expr) assert(expr)
#define ASSERT(expr) \
		if (!(expr)) { \
			        SYS_TimeStampPrintAll(); \
      	  	  	    SYS_TimeStampPrintAllAliased(); \
                    printf("Assertion: (" _STR(expr) "), file " __FILE__ \
                    ", line " _STR(__LINE__) "\n"); \
                    SW_BREAKPOINT; \
                    assert(expr); \
		}


#else
#define ASSERT(expr)
#endif

#ifdef USE_ASSERT
#define ASSERT_PTR_ALIGNED(ptr, factor) assert(!((Uint32)ptr % factor))
#else
#define ASSERT_PTR_ALIGNED(ptr, factor)
#endif



#define ROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))

#define ROUND_UP_N_BYTES_TO_DOUBLE_WORD_MULTIPLE(x) ROUND_UP(x, 8)

typedef Uint32 STAT;
typedef STAT* PSTAT;


static inline Uint32 LOCAL_TO_GLOBAL_ADDRESS(Uint32 loAddr, Uint32 coreId) {
  return (coreId << 24) | 0x10000000 | loAddr;
}

static inline Uint32 IS_ADDRESS_GLOBAL(Uint32 addr) {
  return (addr << 3) >> 31;	//The top hex digit is 1 for a global address
}

static inline Uint32 IS_PTR_ALIGNED(Uint32 *ptr, Uint32 alignmentFactor) {
  return ((Uint32)ptr % alignmentFactor == 0);
}

//returns ceil(a/b)
static inline Uint32 ceil(Uint32 a, Uint32 b) {
	Uint32 q, r;
	q = a/b; r = a - q*b;
//	q = q + _min(1, r);
	q = q + (r > 0);
	return q; 
}

//returns floor(a/b)
static inline Uint32 floor(Uint32 a, Uint32 b) {
	Uint32 q;
	q = a/b;
	return q;
}

#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define DIFF_NONNEG(a,b)  ((a) > (b) ? ((a) - (b)) : 0)

#endif /* _SWPFORM_H_ */
