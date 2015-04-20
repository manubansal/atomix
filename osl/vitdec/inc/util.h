/**
Atomix project, util.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef UTIL_H_
#define UTIL_H_

//#include "std.h"
#include <osl/inc/swpform.h>
#define IN
#define OUT
#define INOUT

//state machine implementation

void invertBitsToBPSK(Int8 *, Uint32);

void printBytesAsBitsUsingLsb(IN Uint8 *buf, IN Uint32 length);

void printOctets(IN Uint8 *buf, IN Uint32 length);
void printOctetsDec(IN Uint8 *buf, IN Uint32 length);
void printBitsMsb(IN Uint32 n);
void printBitsLsb(IN Uint32 n);
void printBitsMsb8(IN Uint8 n);
void printBitsLsb8(IN Uint8 n);
void printSpacedBitsMsb8(IN Uint8 n);
void printSpacedBitstream(IN Uint8 *buf, IN Uint32 length);

void printOctetsFromBytesDec(IN Uint8 *buf, IN Uint32 length);
void print8BitsFrom8BytesOlderFirst(IN Uint8 * arr);

//void printCplx16(IN Cplx16 * buf, IN Uint32 length);

#ifdef _BIG_ENDIAN
void DSP_blk_eswap64
(
    void *src,
    void *dst,
    int  n_dbls
);
#endif

#endif /*UTIL_H_*/
