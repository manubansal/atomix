/**
Atomix project, x86.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#ifndef X86_H
#define X86_H 

#ifdef _X86

//TODO:x86 -- these will work for functional correctness but will
//sacrifice efficiency.
#define _amem4(ptr) (*((long *)(ptr)))
#define _mem4(ptr) (*((long *)(ptr)))

#endif


#endif /* X86_H */
