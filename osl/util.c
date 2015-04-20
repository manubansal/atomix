/**
Atomix project, util.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include "util.h"
#include <string.h>


void zeroOutBits(
		IN	Uint8	outBuf[restrict],
		IN	Uint32	zeroOffset_bits,
		IN	Uint32	zeroSize_bits
		) {	
	
	Uint32 pre, preI, post, postI;
	Uint32 fullBytes, preByte, postByte;
	Uint8 preData, postData;
	
	static Uint8 maskPre[] = {
		0x00,
		0x80,
		0xC0,
		0xE0,
		0xF0,
		0xF8,
		0xFC,
		0xFE,
		0xFF
	};
	
	static Uint8 maskPost[] = {
		0x00,
		0x01,
		0x03,
		0x07,
		0x0F,
		0x1F,
		0x3F,
		0x7F,
		0xFF
	};
	
	preI = zeroOffset_bits % 8; 												//num of bits to preserve in that byte pre zero
	pre = 8 - preI;																//maximum number of pre-bits being zeroed out
	pre = (pre > zeroSize_bits ? zeroSize_bits : pre);							//exact number of pre-bits being zeroed out
	post = (zeroOffset_bits + zeroSize_bits - 1) % 8;							//within-byte index of last bit to zero out (0-7)
	postI = 7 - post;															//num of bits to preserve in the last byte post zero
	fullBytes = (zeroSize_bits - pre) / 8;										//number of full bytes being zeroed out
	
	preByte = zeroOffset_bits/8;												//location of pre byte
	postByte = (zeroOffset_bits + zeroSize_bits)/8;								//locate of post byte (may be same as above)
	
	preData = outBuf[preByte] & maskPre[preI];
	postData = outBuf[postByte] & maskPost[postI];
	
	//the following order of assignments gives correct behavior even when preByte == postByte
	outBuf[preByte] = 0;
	outBuf[postByte] = 0;
	outBuf[preByte] |= preData;
	outBuf[postByte] |= postData;
	
	memset(outBuf + preByte + 1, 0, fullBytes);
}

#include "util.h"
#include "stdio.h"

void invertBitsToBPSK(Int8 *SoftDePuncturedBits, Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		SoftDePuncturedBits[i] = SoftDePuncturedBits[i] ? -1 : 1;
	}
}

void printBytesAsBitsUsingLsb(IN Uint8 *buf, IN Uint32 length) {
	int i;
	for (i = 1; i <= length; i++) {
		if (buf[i-1] & 1)
		  printf("1");
		else
		  printf("0");
		if (i % 8 == 0)
		  printf(" ");
		if (i % 64 == 0)
		  printf("\n");
	}
	printf("\n");
}

void printOctets(IN Uint8 *buf, IN Uint32 length) {
	int i, j;
	int nr = length/4;
	int nc = length % 4;
	for (i = 0; i < nr; i++) {
		for (j = 0; j < 4; j++) {
			printf("%02X ", buf[i * 4 + j]);
		}
		printf("\n");
	}
	for (j = 0; j < nc; j++) {
		printf("%02X ", buf[i * 4 + j]);
	}
	printf("\n");
}

void printOctetsDec(IN Uint8 *buf, IN Uint32 length) {
	int i, j;
	int nr = length/4;
	int nc = length % 4;
	for (i = 0; i < nr; i++) {
		for (j = 0; j < 4; j++) {
			printf("%04d ", buf[i * 4 + j]);
		}
		printf("\n");
	}
	for (j = 0; j < nc; j++) {
		printf("%04d ", buf[i * 4 + j]);
	}
	printf("\n");
}

void printOctetsDecSigned(IN Int8 *buf, IN Uint32 length) {
	int i, j;
	int nr = length/4;
	int nc = length % 4;
	for (i = 0; i < nr; i++) {
		printf("%04d  ", i * 4);
		for (j = 0; j < 4; j++) {
			printf("%04d ", buf[i * 4 + j]);
		}
		printf("\n");
	}
	for (j = 0; j < nc; j++) {
		printf("%04d ", buf[i * 4 + j]);
	}
	printf("\n");
}


void printOctetsFromBytesDec(IN Uint8 *buf, IN Uint32 length) {
	int i, j, k;
	Uint8 currOct = 0;
	int nr = length/4;
	int nc = length % 4;
	for (i = 0; i < nr; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 8; k++) {
				currOct <<= 1;
				currOct |= buf[(i * 4 + j) * 8 + k];
			}
			//printf("%04d ", buf[i * 4 + j]);
			printf("%04d ", currOct);
		}
		printf("\n");
	}
	for (j = 0; j < nc; j++) {
		printf("%04d ", buf[i * 4 + j]);
	}
	printf("\n");
}

void printSpacedBitstream(IN Uint8 *buf, IN Uint32 length) {
	int i;//, j;
	int nr = length;///4;
//	int nc = 1;//length % 4;
	for (i = 0; i < nr; i++) {
//		for (j = 0; j < nc; j++) {
			//printf("%02X ", buf[i * 4 + j]);
//			printSpacedBitsMsb8(buf[i * 4 + j]);
			printSpacedBitsMsb8(buf[i]);
//		}
		printf("\n");
	}
//	for (j = 0; j < nc; j++) {
//		printf("%02X ", buf[i * 4 + j]);
//	}
	printf("\n");
}


void printBitsMsb(IN Uint32 n) {
	Uint32 j = 0;
	Uint64 i = 1;
	i <<= (sizeof(n) * 8 - 1);

	while (i > 0) {
		if (n & i)
			printf("1");
		else
			printf("0");
		i >>= 1;
		j++;
		if (j % 8 == 0)
		  printf(" ");
	}
	printf(" ");
}

void printBitsLsb(IN Uint32 n) {
	Uint32 i, j = 0;
	//i = 1<<(sizeof(n) * 8 - 1);
	i = 1;

	//while (i > 0) {
	while (j < sizeof(n) * 8) {
		if (n & i)
			printf("1");
		else
			printf("0");
		//i >>= 1;
		i <<= 1;
		j++;
		if (j % 8 == 0)
		  printf(" ");
	}
	printf(" ");
}

void printBitsMsb8(IN Uint8 n) {
	Uint32 i;
	i = 1<<(sizeof(n) * 8 - 1);

	while (i > 0) {
		if (n & i)
			printf("1");
		else
			printf("0");
		i >>= 1;
	}
	printf(" ");
}

/* print the 8 bits contained in 8 input bytes, bit in lower byte printed first */
void print8BitsFrom8BytesOlderFirst(IN Uint8 * arr) {
	Uint32 i;
	Uint8 n;

	for (i = 0; i < 8; i++) {
		n <<= 1;
		n |= arr[i];
	}
	i = 1<<(sizeof(n) * 8 - 1);

	while (i > 0) {
		if (n & i)
			printf("1");
		else
			printf("0");
		i >>= 1;
	}
	printf(" ");
}

void printSpacedBitsMsb8(IN Uint8 n) {
	Uint32 i;
	i = 1<<(sizeof(n) * 8 - 1);

	while (i > 0) {
		if (n & i)
			printf("1");
		else
			printf("0");
		i >>= 1;
		printf(" ");
	}
	printf(" ");
}

void printBitsLsb8(IN Uint8 n) {
	Uint32 i, j = 0;
	//i = 1<<(sizeof(n) * 8 - 1);
	i = 1;

	//while (i > 0) {
	//while (i <= 1<<(sizeof(n) * 8 - 1)) {
	while (j < sizeof(n) * 8) {
		if (n & i)
			printf("1");
		else
			printf("0");
		//i >>= 1;
		i <<= 1;
		j++;
	}
	printf(" ");
}

void printCplx16(IN Cplx16 * buf, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 06d % 06d    ", i, buf[i].real, buf[i].imag);
	    if (i % 4 == 3)
	      printf("\n");
	}
	printf("\n");
}

void printCplx16Scale8(IN Cplx16 * buf, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 06d % 06d    ", i, (Int8)buf[i].real, (Int8)buf[i].imag);
	    if (i % 4 == 3)
	      printf("\n");
	}
	printf("\n");
}


void printCplx32(IN Cplx32 * buf, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 010d % 010d    ", i, buf[i].real, buf[i].imag);
	    if (i % 4 == 3)
	      printf("\n");
	}
	printf("\n");
}

void printCplx16SingleColumn(IN Cplx16 * buf, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 06d % 06d    \n", i, buf[i].real, buf[i].imag);
//	    if (i % 4 == 3)
//	      printf("\n");
	}
	printf("\n");
}

void printCplx32SingleColumn(IN Cplx32 * buf, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 014d % 014d    \n", i, buf[i].real, buf[i].imag);
//	    if (i % 4 == 3)
//	      printf("\n");
	}
	printf("\n");
}


void printCplx16Adjacent(IN Cplx16 * buf, IN Cplx16 * buf2, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 07d % 07d    |  %07d  % 07d\n", i, buf[i].real, buf[i].imag, buf2[i].real, buf2[i].imag);
//	    if (i % 4 == 3)
//	      printf("\n");
	}
	printf("\n");
}


void printUint32Adjacent(IN Uint32 * buf, IN Uint32 * buf2, IN Uint32 length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   %14d    |  %14d\n", i, buf[i], buf2[i]);
//	    if (i % 4 == 3)
//	      printf("\n");
	}
	printf("\n");
}

void printUint32AdjacentDiff(IN Uint32 * buf, IN Uint32 * buf2, IN Uint32 length) {
	int i;
	printf("sno.   buf1vals     |  buf2vals         ||      difference\n");
	printf("xxxx   xxxxxxxxxx   |  xxxxxxxxxx       ||      xxxxxxxxxx\n");
	for (i = 0; i < length; i++) {
		printf("%04d   %14d   |  %14d       ||      %14d\n", i, buf[i], buf2[i], (Int32)buf[i] - (Int32)buf2[i]);
//	    if (i % 4 == 3)
//	      printf("\n");
	}
	printf("\n");
}


void printCplx16_scaledDown(IN Cplx16 * buf, IN Uint32 length, IN Uint32 shift) {
	int i;
	for (i = 0; i < length; i++) {
		printf("%04d   % 06d, % 06d    ", i, buf[i].real >> shift, buf[i].imag >> shift);
	    if (i % 4 == 3)
	      printf("\n");
	}
	printf("\n");
}

Uint32 vectorSumUint32(IN Uint32 *buf, IN Uint32 len) {
	int i;
	Uint32 sum = 0;
	for (i = 0; i < len; i++) {
		sum += buf[i];
	}
	return sum;
}

//Uint64 vectorDiffInt32(IN Int32 *buf1, IN Int32 *buf2, OUT Int32 *diff, IN Uint32 len) {
//	int i;
//	Uint64 normsq = 0;
//	for (i = 0; i < len; i++) {
//		diff[i] = buf1[i] - buf2[i];
//		normsq += (Int64)diff[i] * (Int64)diff[i];
//	}
//	return normsq;
//}

void util_swapBitsInBytes(Uint8 * inpBytes, Uint8 * outBytes, Uint32 nBytes) {
  int i;
  for (i = 0; i < nBytes; i++) {
    Uint32 v = inpBytes[i];
    v = _bitr(v) >> 24;
    outBytes[i] = v;
  }
}
