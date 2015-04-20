/**
Atomix project, amem4bitcpy.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Feb 14, 2012
*/

#ifndef AMEM4BITCPY_H_
#define AMEM4BITCPY_H_

#include "or_types.h"
#include "swpform.h"


static inline void amem4bitcpy(
		OUT	Uint32 * const restrict outBuf,
		IN	Uint32 const * const restrict inBuf,
		IN	Uint32 const nBitsToCopy,
		IN	Uint32 const nInpBitsToSkip
		) {
	ASSERT_PTR_ALIGNED(inBuf, 4);
	ASSERT_PTR_ALIGNED(outBuf, 4);


	Uint32 i, j;

	Uint32 n_words_to_skip = nInpBitsToSkip >> 5;			//nBitsToSkip/32;
	Uint32 n_bits_to_skip = nInpBitsToSkip & 0x0000001F;		//nBitsToSkip % 32;
	Uint32 n_words_to_copy = nBitsToCopy >> 5;
	Uint32 n_bits_to_copy = nBitsToCopy & 0x0000001F;
	Uint32 n_bits_to_skip_c = 32 - n_bits_to_skip;

	Uint32 bufWord = 0;
	Uint32 word_to_preserve_from;

	//copy in whole-words worth of bits
	for (i = n_words_to_skip, j = 0; i < n_words_to_skip + n_words_to_copy; i++, j++) {
		bufWord = _amem4(&inBuf[i]) << n_bits_to_skip;
		bufWord |= _amem4(&inBuf[i+1]) >> n_bits_to_skip_c;
		_amem4(&outBuf[j]) = bufWord;
	}

	//copy in the last partial word while preserving those bits that are not being replaced
	//while ensuring that the input buffer is only accessed to the last word that is actually
	//required, but no extra word beyond that.
	bufWord = _amem4(&inBuf[i]) << n_bits_to_skip;
	bufWord = n_bits_to_skip_c < n_bits_to_copy ?
			(bufWord |= _amem4(&inBuf[i+1]) >> n_bits_to_skip_c)  :
			(bufWord &= 0xFFFFFFFF << (32 - n_bits_to_copy));

	bufWord &= 0xFFFFFFFF << (32 - n_bits_to_copy);
	word_to_preserve_from = _amem4(&outBuf[j]);
	bufWord |= (word_to_preserve_from & (0xFFFFFFFF >> (n_bits_to_copy)));
	_amem4(&outBuf[j]) = bufWord;
}

static inline void amem4bitcpy_outskip(
		OUT	Uint32 * const restrict outBuf,
		IN	Uint32 const * const restrict inBuf,
		IN	Uint32 const nBitsToCopy,
		IN	Uint32 const nOutBitsToSkip,
		IN	Uint32 const nInpBitsToSkip
		) {
	ASSERT_PTR_ALIGNED(inBuf, 4);
	ASSERT_PTR_ALIGNED(outBuf, 4);

	Uint32 n_out_words_to_skip = nOutBitsToSkip >> 5;			//nBitsToSkip/32;
	Uint32 n_out_bits_to_skip = nOutBitsToSkip & 0x0000001F;		//nBitsToSkip % 32;
//	Uint32 n_words_to_copy = nBitsToCopy >> 5;
//	Uint32 n_bits_to_copy = nBitsToCopy & 0x0000001F;
	Uint32 n_out_bits_to_skip_c = 32 - n_out_bits_to_skip;

	Uint32 n_inp_words_to_skip = nInpBitsToSkip >> 5;			//nBitsToSkip/32;
	Uint32 n_inp_bits_to_skip = nInpBitsToSkip & 0x0000001F;		//nBitsToSkip % 32;

	Uint32 n_i_s = nInpBitsToSkip + n_out_bits_to_skip_c;
	Uint32 n_b_c = nBitsToCopy > n_out_bits_to_skip_c ? nBitsToCopy - n_out_bits_to_skip_c : 0;
	if (n_b_c)
		amem4bitcpy(
				&outBuf[n_out_words_to_skip + 1],
				inBuf,
				n_b_c,
				n_i_s
				);
	Uint32 saved_out_word = outBuf[n_out_words_to_skip];
	Uint32 saved_out_word_copy = saved_out_word;
	Uint32 saved_inp_word1 = 0, saved_inp_word2 = 0;
	if (n_out_bits_to_skip >= n_inp_bits_to_skip) {
		saved_out_word >>= n_out_bits_to_skip_c;
		saved_out_word <<= n_out_bits_to_skip_c;
		saved_inp_word1 = _amem4(&inBuf[n_inp_words_to_skip]);
		saved_inp_word1 <<= n_inp_bits_to_skip;
		saved_inp_word1 >>= (n_out_bits_to_skip);
		saved_out_word |= saved_inp_word1;
	}
	else {
		saved_out_word >>= n_out_bits_to_skip_c;
		saved_out_word <<= n_out_bits_to_skip_c;
		saved_inp_word1 = _amem4(&inBuf[n_inp_words_to_skip]);
		saved_inp_word1 <<= n_inp_bits_to_skip;
		saved_inp_word1 >>= n_out_bits_to_skip;
		saved_inp_word2 = _amem4(&inBuf[n_inp_words_to_skip + 1]);
		saved_inp_word2 >>= n_out_bits_to_skip + 32 - n_inp_bits_to_skip;
		saved_out_word |= saved_inp_word1;
		saved_out_word |= saved_inp_word2;
	}


	if (n_out_bits_to_skip + nBitsToCopy < 32) {
		Uint32 n_to_touch = n_out_bits_to_skip + nBitsToCopy;
		Uint32 n_to_touch_c = 32 - n_to_touch;
		saved_out_word >>= n_to_touch_c;
		saved_out_word <<= n_to_touch_c;
		saved_out_word_copy <<= n_to_touch;
		saved_out_word_copy >>= n_to_touch;
		saved_out_word |= saved_out_word_copy;
	}

	_amem4(&outBuf[n_out_words_to_skip]) = saved_out_word;

}

#endif /* AMEM4BITCPY_H_ */
