
; Registers use:

; xmm0	-> Plaintext
; xmm1	-> ShiftRows_Mask


shiftRows:
	pshufb			xmm0, xmm2

	ret
