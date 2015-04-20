/**
Atomix project, setup.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef SETUP_H_
#define SETUP_H_

#include <osl/inc/swpform.h>

//to pass vitdec constants to atoms.txt/states.txt
#include <osl/vitdec/inc/vitdec/vitdec_profile.h>

#ifndef LARGE_TRACE_MANLOADED
extern far const Int16 INPUT_TRACE_ARRAY_NAME[];
#endif

extern far const Int16 DSTMACADDR_ARRAY_NAME[];
extern Int16 wifilib_ack_preamble_signal[];

#define TX_BUFFER_SIZE_BYTES	3520*2
//#define TX_BUFFER_N_WRAPAROUNDS	0
#define TX_BUFFER_N_WRAPAROUNDS	MAXUINT32
extern far Uint8 TX_BUFFER[];


#endif

