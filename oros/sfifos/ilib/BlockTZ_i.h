/**
Atomix project, BlockTZ_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef BLOCKTZ_I_H_
#define BLOCKTZ_I_H_

void BlockTZ_i (
	IN  FIFO_BufferState    * bh_src,
	OUT FIFO_BufferState    * bh_dst,
	CF  BlockTZ_Conf  	* conf
	);



void BlockTZ_i_conf (
	CF BlockTZ_Conf * conf,
	IN Uint32 lengthInDblWords,
	IN Uint32 linkNum
	);


#endif

