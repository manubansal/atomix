/**
Atomix project, SYS_TimeStamp.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef SYS_TIMESTAMP_H_
#define SYS_TIMESTAMP_H_

#include <osl/inc/swpform.h>

void SYS_TimeStamp_0(Uint64 id);
void SYS_TimeStamp_1(Uint64 id);
void SYS_TimeStamp_2(Uint64 id);
void SYS_TimeStamp_3(Uint64 id);

void SYS_TimeStamp_aliased(Uint64 id);

void SYS_TimeStampPrintAll();
void SYS_TimeStampPrintAllAliased();

#endif

