/**
Atomix project, util.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef UTIL_H_
#define UTIL_H_

//#include "std.h"
#include "swpform.h"
#include "or_types.h"

//state machine implementation

void invertBitsToBPSK(Int8 *, Uint32);

void printBytesAsBitsUsingLsb(IN Uint8 *buf, IN Uint32 length);

void printOctets(IN Uint8 *buf, IN Uint32 length);
void printOctetsDec(IN Uint8 *buf, IN Uint32 length);
void printOctetsDecSigned(IN Int8 *buf, IN Uint32 length);
void printBitsMsb(IN Uint32 n);
void printBitsLsb(IN Uint32 n);
void printBitsMsb8(IN Uint8 n);
void printBitsLsb8(IN Uint8 n);
void printSpacedBitsMsb8(IN Uint8 n);
void printSpacedBitstream(IN Uint8 *buf, IN Uint32 length);

void printOctetsFromBytesDec(IN Uint8 *buf, IN Uint32 length);
void print8BitsFrom8BytesOlderFirst(IN Uint8 * arr);

void printCplx16(IN Cplx16 * buf, IN Uint32 length);
void printCplx16Scale8(IN Cplx16 * buf, IN Uint32 length);
void printCplx32(IN Cplx32 * buf, IN Uint32 length);
void printCplx16_scaledDown(IN Cplx16 * buf, IN Uint32 length, IN Uint32 factor);

void printCplx16SingleColumn(IN Cplx16 * buf, IN Uint32 length);
void printCplx132SingleColumn(IN Cplx32 * buf, IN Uint32 length);
void printCplx16Adjacent(IN Cplx16 * buf, IN Cplx16 * buf2, IN Uint32 length);
void printUint32Adjacent(IN Uint32 * buf, IN Uint32 * buf2, IN Uint32 length);
void printUint32AdjacentDiff(IN Uint32 * buf, IN Uint32 * buf2, IN Uint32 length);

Uint32 vectorSumUint32(IN Uint32 *buf, IN Uint32 len);
Uint64 vectorDiffInt32(IN Int32 *buf1, IN Int32 *buf2, OUT Int32 *diff, IN Uint32 len);

void zeroOutBits(
		IN	Uint8	outBuf[restrict],
		IN	Uint32	zeroOffset_bits,
		IN	Uint32	zeroSize_bits
		);
#ifdef _BIG_ENDIAN
void DSP_blk_eswap64
(
    void *src,
    void *dst,
    int  n_dbls
);
#endif

//void printCplx16(Cplx16 a);

static inline Uint32 reverseBitsInBytes32(IN Uint32 in32) {
	return _rotl(_swap4(_bitr(in32)), 16);
}

#endif /*UTIL_H_*/
