/**
Atomix project, LOG_logger.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <c6x.h>

#ifdef LOG_CONSOLE
#ifndef LOG_STDOUT_FILE_PREFIX
#error: no LOG_STDOUT_FILE_PREFIX defined
#endif
#endif

/* 
 * must define LOG_CONSOLE to use this module
 * must define LOG_STDOUT_FILE_PREFIX to use this module
 */

#ifdef LOG_CONSOLE

FILE** LOG_getLogFd() {
  static far FILE * fid = NULL;
  return &fid;
}

void LOG_setup() {
  Uint32 coreId = DNUM;
  char logfile[sizeof(LOG_STDOUT_FILE_PREFIX) + 5];
  sprintf(logfile, "%s_%d", LOG_STDOUT_FILE_PREFIX, coreId);
  FILE ** fid = LOG_getLogFd();
  *fid = fopen(logfile, "a");
}

void LOG_close() {
  FILE ** fid = LOG_getLogFd();
  fclose(*fid);
}

#endif
