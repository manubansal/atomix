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

//TODO:x86 -- we don't need to use the far keyworld on x86
#define far


//TODO:x86 -- for reading the 64-bit time stamp counter

#if defined(__i386__)

//static __inline__ unsigned long long rdtsc(void)
static __inline__ unsigned long long CSL_tscRead(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

#elif defined(__x86_64__)

//static __inline__ unsigned long long rdtsc(void)
static __inline__ unsigned long long CSL_tscRead(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#endif


#endif


#endif /* X86_H */
