/**
Atomix project, amem4bitcpy.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Feb 14, 2012
*/

#include <or_types.h>
#include <swpform.h>



//#pragma DATA_ALIGN(testInp, 4);
//#pragma DATA_ALIGN(testOut, 4);
//Uint8 testInp[8] = {
//		0xF1, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
//};
//
//Uint8 testOut[12] = {
//		0xEF, 0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE, 0x48, 0x39, 0x92
//};

//void test_amem4bitcpy_i(
//		Uint32 nBitsToCopy,
//		Uint32 nBitsToSkip
//		) {
//	Uint32 i;
//
//	printf("nBitsToCopy: %d,  nBitsToSkip: %d\n", nBitsToCopy, nBitsToSkip);
//	for (i= 0; i < 8; i++)
//		printBitsMsb8(testInp[i]);
//	printf("\n");
//
//	for (i= 0; i < 12; i++)
//		printBitsMsb8(testOut[i]);
//	printf("\n");
//
//	amem4bitcpy(
//			(Uint32 *)testOut,
//			(Uint32 *)testInp,
//			nBitsToCopy,
//			nBitsToSkip
//	);
//
//	for (i= 0; i < 12; i++)
//		printBitsMsb8(testOut[i]);
//	printf("\n\n");
//}

/*
 * All tests tried below pass. There may still be corner cases that I have missed.
 */
//void test_amem4bitcpy() {
//
//	Uint32 nBitsToCopy;
//	Uint32 nBitsToSkip;
//
//	//test1 --------------------------------------------------------------------
//	nBitsToCopy = 11;
//	nBitsToSkip = 5;
//
//	printf("test1\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test2 --------------------------------------------------------------------
//	nBitsToCopy = 11;
//	nBitsToSkip = 0;
//
//	printf("test2\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test3 --------------------------------------------------------------------
//	nBitsToCopy = 32;
//	nBitsToSkip = 0;
//
//	printf("test3\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test4 --------------------------------------------------------------------
//	nBitsToCopy = 32;
//	nBitsToSkip = 32;
//
//	printf("test4\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test5 --------------------------------------------------------------------
//	nBitsToCopy = 16;
//	nBitsToSkip = 24;
//
//	printf("test5\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test6 --------------------------------------------------------------------
//	nBitsToCopy = 40;
//	nBitsToSkip = 18;
//
//	printf("test6\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test7 --------------------------------------------------------------------
//	nBitsToCopy = 0;
//	nBitsToSkip = 18;
//
//	printf("test7\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//
//	//test8 --------------------------------------------------------------------
//	nBitsToCopy = 4;
//	nBitsToSkip = 31;
//
//	printf("test8\n");
//	test_amem4bitcpy_i(nBitsToCopy, nBitsToSkip);
//	//---------------------------------------------------------------------------
//}
