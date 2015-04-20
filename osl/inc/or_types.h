/**
Atomix project, or_types.h, This header file declares all useful basic types for the ORLIB library.
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Author(s): Jeffrey Mehlman
*/

#ifndef OR_TYPES_H_
#define OR_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* *****************************************************************************
 *			Public Includes
 * ************************************************************************** */
#include <ti/csl/tistdtypes.h>
#include <stdint.h>
/* *****************************************************************************
 *			Type Definitions
 * ************************************************************************** */


/** 
 * For the method below, brief purpose description.
 */

/* Define all the basic types we'll need */
//typedef unsigned char 		uint8_t;
//typedef unsigned short 		uint16_t;
//typedef unsigned int 		uint32_t;
//typedef unsigned long long	uint64_t;

/* The following types are already defined in stdint.h.  --MB
typedef char			int8_t;
typedef short			int16_t;
typedef int 			int32_t;
typedef long long		int64_t;
*/

typedef	unsigned char		bool_t;

/* Define old types for backwards compatibility */
//typedef unsigned char 		Uint8;
//typedef unsigned short 		Uint16;
//typedef unsigned int 		Uint32;
typedef unsigned long long	Uint64;
//
//typedef char			Int8;
//typedef short			Int16;
//typedef int 			Int32;
typedef long long		Int64;
//
//typedef	unsigned char		Bool;
#define OR_TRUE 1
#define OR_FALSE 0

/* Define the complex types */
typedef struct Cplx8
{
	int8_t real;
	int8_t imag;
} Cplx8;

typedef struct Cplx16
{
	int16_t real;
	int16_t imag;
} Cplx16;

typedef struct Cplx32
{
	int32_t real;
	int32_t imag;
} Cplx32;

typedef union Cplx16u
{
	Cplx16 	cplx16;
	int32_t		realimag;
} Cplx16U;

typedef union {
  struct {
    Cplx16U c1;
    Cplx16U c2;
  } c;

  Uint64  u64;
} Cplx16U2;


typedef union Cplx32u
{
	Cplx32 	cplx32;
	int64_t		realimag;
} Cplx32U;

/* Define Special Io types */
typedef uint8_t 	BitsPacked;
typedef int8_t 		BitsSoft;
typedef int8_t 		SoftBit;
typedef uint64_t	BitsPacked_ll;
typedef uint8_t		BitsUnpacked;
typedef Cplx16		ComplexSamples;

/* Useful Macros */
#define IN
#define OUT
#define INOUT


#ifdef __cplusplus
} //extern C
#endif

#endif /* ORLIB_TYPES_H_ */

/* ============================================================================
*            Copyright (c) 2011 Stanford University.          
*                           All Rights Reserved.                          
* ========================================================================== */
