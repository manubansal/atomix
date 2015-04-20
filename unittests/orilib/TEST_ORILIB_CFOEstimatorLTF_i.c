/**
Atomix project, TEST_ORILIB_CFOEstimatorLTF_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/OSL_debug.h>
#include <orilib/ORILIB_CFOEstimatorLTF_t.h> 
#include <orilib/ORILIB_CFOEstimatorLTF_i.h> 

#define N_TEST_VECTORS	10	

#pragma DATA_SECTION(TEST_ORILIB_CFOEstimatorLTF_i_IN_inpFullLtfBuf, ".data");
#pragma DATA_SECTION(TEST_ORILIB_CFOEstimatorLTF_i_IN_agcState, ".data");
#pragma DATA_ALIGN(TEST_ORILIB_CFOEstimatorLTF_i_IN_inpFullLtfBuf, 8);
#pragma DATA_ALIGN(TEST_ORILIB_CFOEstimatorLTF_i_IN_agcState, 8);
IN  ORILIB_t_Cplx16Buf160 TEST_ORILIB_CFOEstimatorLTF_i_IN_inpFullLtfBuf[N_TEST_VECTORS];
IN  ORILIB_t_PktAgcState  TEST_ORILIB_CFOEstimatorLTF_i_IN_agcState[N_TEST_VECTORS];

#pragma DATA_SECTION(TEST_ORILIB_CFOEstimatorLTF_i_OUT_cfoState, ".data");
#pragma DATA_ALIGN(TEST_ORILIB_CFOEstimatorLTF_i_OUT_cfoState, 8);
OUT ORILIB_t_CFOState TEST_ORILIB_CFOEstimatorLTF_i_OUT_cfoState[N_TEST_VECTORS];

int TEST_ORILIB_CFOEstimatorLTF_i() {

  LOG_PRINTF("TEST_ORILIB_CFOEstimatorLTF_i.c begin\n");

  IN  ORILIB_t_Cplx16Buf160 * inpFullLtfBuf;
  IN  ORILIB_t_PktAgcState * agcState;
  OUT ORILIB_t_CFOState * cfoState;

  int i;
  for (i = 0; i < N_TEST_VECTORS; i++) {

    inpFullLtfBuf = &TEST_ORILIB_CFOEstimatorLTF_i_IN_inpFullLtfBuf[i];
    agcState = &TEST_ORILIB_CFOEstimatorLTF_i_IN_agcState[i];

    cfoState = &TEST_ORILIB_CFOEstimatorLTF_i_OUT_cfoState[i];

    //OSL_dumpBuffer(bli->atomid, bli->debug, "bhi0", bhi0->mem, sizeof(ORILIB_t_Cplx16Buf160));
    //OSL_dumpBuffer(bli->atomid, bli->debug, "bhi1", bhi1->mem, sizeof(ORILIB_t_PktAgcState));


    //ORILIB_CFOEstimatorLTF_i(
    //  (ORILIB_t_Cplx16Buf160 *)(bhi0->mem),
    //  (ORILIB_t_PktAgcState *)(bhi1->mem),
    //  (ORILIB_t_CFOState *)(bho0->mem)
    //);
    ORILIB_CFOEstimatorLTF_i(
	  inpFullLtfBuf,
	  agcState,
	  cfoState
	  );

    //OSL_dumpBuffer(bli->atomid, bli->debug, "bho0", bho0->mem, sizeof(ORILIB_t_CFOState));
    OSL_dumpBuffer(117, 1, "bho0", cfoState, sizeof(ORILIB_t_CFOState));
  }


  LOG_PRINTF("TEST_ORILIB_CFOEstimatorLTF_i.c end\n");

  return 0;
}


