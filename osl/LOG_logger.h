/**
Atomix project, LOG_logger.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef _LOG_LOGGER_H_
#define _LOG_LOGGER_H_

#ifdef LOG_CONSOLE

FILE** LOG_getLogFd();
void LOG_setup();
void LOG_close();

#define LOG_PRINTF(...) fprintf(*LOG_getLogFd(), __VA_ARGS__)

#else

#define LOG_PRINTF(...) printf(__VA_ARGS__)

#endif

#endif //_LOG_LOGGER_H_
