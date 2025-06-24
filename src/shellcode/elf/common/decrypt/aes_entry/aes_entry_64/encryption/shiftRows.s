; Registers use:

; xmm0	-> Plaintext
; xmm1	-> ShiftRows_Mask


shiftRows:
	vextracti128	xmm3, ymm2, 0
	pshufb			xmm0, xmm3

	ret
