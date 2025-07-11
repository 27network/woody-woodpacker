; Registers use:

; xmm0	-> Ciphertext
; rsi	-> Key (expanded)
; rcx	-> Round number


addRoundKey:
	shl	rcx, 4
	movdqa	xmm3, [rsi + rcx]
	pxor	xmm0, xmm3
	shr	rcx, 4

	ret
