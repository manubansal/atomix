/**
Atomix project, main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/LOG_logger.h>

int TEST_ORILIB_CFOEstimatorLTF_i();

int main() {
#ifdef LOG_CONSOLE
  LOG_setup();
#endif

  TEST_ORILIB_CFOEstimatorLTF_i();

#ifdef LOG_CONSOLE
  LOG_close();
#endif
}
