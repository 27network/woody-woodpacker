BITS 64

%include "includes/sections.s"

global addRoundKey


; Registers use:

; xmm0	-> Plaintext
; rsi	-> Key (expanded)
; rcx	-> Round number


addRoundKey:
	shl		rcx, 4
	movdqa	xmm3, [rsi + rcx]
	pxor	xmm0, xmm3
	shr		rcx, 4

	ret
