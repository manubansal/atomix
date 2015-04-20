/**
Atomix project, vitc_api.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jan 8, 2012
*/

#ifndef VITC_API_H_
#define VITC_API_H_

#include "swpform.h"

void viterbiDecodeC(
        Uint8 BranchMetrics[],
        Uint32 OutputBits32[],
        Uint32 NumInfoBits
        );

#endif /* VITC_API_H_ */
