/**
Atomix project, OSL_debug_pktdataout.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef OSL_DEBUG_PKTDATAOUT_H
#define OSL_DEBUG_PKTDATAOUT_H 

#define SNRBER
#ifdef SNRBER
//extern FILE * fp_pktdataout;
extern far Uint32 pktdataout_nbyteswritten;
extern Uint8 pktdataout[];
#endif


#endif /* OSL_DEBUG_PKTDATAOUT_H */
