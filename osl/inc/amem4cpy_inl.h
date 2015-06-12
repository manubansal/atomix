/**
Atomix project, amem4cpy_inl.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Feb 23, 2012
*/

#ifndef AMEM4CPY_INL_H_
#define AMEM4CPY_INL_H_

#include <osl/inc/x86.h>

static inline void _amem4cpy(void * restrict dst, void * restrict src, unsigned int numberOfWords) {
	unsigned int i = 0;
	for (i = 0; i < numberOfWords; i++) {
		_amem4(&((unsigned int *)dst)[i]) = _amem4(&((unsigned int *)src)[i]);
	}
}

static inline void _mem4cpy(void * restrict dst, void * restrict src, unsigned int numberOfWords) {
	unsigned int i = 0;
	for (i = 0; i < numberOfWords; i++) {
		_mem4(&((unsigned int *)dst)[i]) = _mem4(&((unsigned int *)src)[i]);
	}
}


#endif /* AMEM4CPY_INL_H_ */
