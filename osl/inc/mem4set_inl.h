/**
Atomix project, mem4set_inl.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Dec 20, 2012
*/


#ifndef MEM4SET_INL_H_
#define MEM4SET_INL_H_

#include "swpform.h"

static inline void _mem4set(void * restrict dst, Uint32 nWordsToSet, Uint32 wordToSetTo) {
	Uint32 i = 0;
	for (i = 0; i < nWordsToSet; i++) {
		_mem4(&((Uint32 *)dst)[i]) = wordToSetTo;
	}
}

static inline void _amem4set(void * restrict dst, Uint32 nWordsToSet, Uint32 wordToSetTo) {
	Uint32 i = 0;
	for (i = 0; i < nWordsToSet; i++) {
		_amem4(&((Uint32 *)dst)[i]) = wordToSetTo;
	}
}


#endif /* MEM4SET_INL_H_ */
