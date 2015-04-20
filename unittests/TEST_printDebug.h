/**
Atomix project, TEST_printDebug.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Feb 8, 2012
*/

#ifndef TEST_PRINTDEBUG_H_
#define TEST_PRINTDEBUG_H_

#include <stdio.h>
#include <osl/inc/or_types.h>
#include <orlib/orlib_blocks/orlib_types_wifi.h>
#include "wifi_parameters.h"


void TEST_printDebug_txRxPktState(TxRxPktState *txRxPktState);
void TEST_printDebug_descrambledBits(Uint8 *bytes, Uint32 nBytes);

#endif /* TEST_PRINTDEBUG_H_ */
