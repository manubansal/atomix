/**
Atomix project, BlockTD_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef BLOCKTD_I_H_
#define BLOCKTD_I_H_

void BlockTD_i (
	RAWIN  FIFO_BufferState   * bh_src,
	IN     BlockTD_State	  * inpState,
	RAWOUT FIFO_BufferState   * bh_dst,
	OUT    BlockTD_State	  * outState,
	CF     BlockTD_Conf 	  * conf
	);

void BlockTD_i_conf (
	CF BlockTD_Conf * conf,
	IN Uint32 nLinks,
	IN Uint32 linkNumStart
	);


#endif

