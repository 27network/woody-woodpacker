BITS 64

%include "includes/defines.s"

global	invShiftRows
global	invShiftRows_32bits_words

invShiftRows:
	vextracti128	xmm3, ymm2, 1
	pshufb			xmm0, xmm3

	ret
