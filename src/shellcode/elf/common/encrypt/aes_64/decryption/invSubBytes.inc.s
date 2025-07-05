BITS 64

%include "includes/sections.s"

global	invSubBytes
global	invSubBytes_32bits_words

invSubBytes:
	vpmovzxbd		ymm6, xmm0	;ymm6 = lower parts of xmm0 (8 bytes)
	psrldq			xmm0, 8
	vpmovzxbd		ymm7, xmm0	;ymm7 - higher parts of xmm0 (8 bytes)

	;extracting 16 substitute bytes from invSbox
	vpcmpeqd		ymm3, ymm3, ymm3
	push			rax
	lea				rax, [rel inv_s_box]
	vpgatherdd		ymm4, [rax + ymm6 * 4], ymm3	;lower part substitution
	vpcmpeqd		ymm3, ymm3, ymm3
	vpgatherdd		ymm5, [rax + ymm7 * 4], ymm3	;higher part substitution
	pop				rax

	;insert first 8 substitue bytes from invSbox
	vextracti128	xmm0, ymm5, 1	;extract higher part of ymm5 into xmm0
	pshufb			xmm0, xmm1
	pslldq			xmm0, 4
	vextracti128	xmm6, ymm5, 0	;extract lower part of ymm5 into xmm6
	pshufb			xmm6, xmm1
	paddb			xmm0, xmm6
	pslldq			xmm0, 8

	;insert last 8 subsitute bytes frm invSbox
	vextracti128	xmm6, ymm4, 1	;extract higher part of ymm4 into xmm6
	pshufb			xmm6, xmm1
	pslldq			xmm6, 4
	vextracti128	xmm7, ymm4, 0	;extract lower part of ymm4 into xmm7
	pshufb			xmm7, xmm1
	paddb			xmm6, xmm7
	paddb			xmm0, xmm6

	ret





;	push	rcx
;	xor		rcx, rcx
;	call	invSubBytes_loop
;	pop		rcx
;
;	ret
;
;invSubBytes_loop:
;	mov		eax, [rdi + 4 * rcx]
;	call	invSubBytes_32bits_words
;	bswap	eax
;	mov		[rdi + 4 * rcx], eax
;
;	inc		rcx
;	cmp		rcx, Nb
;	jl		invSubBytes_loop
;
;	ret
;
;invSubBytes_32bits_words:
;	push	rcx
;	xor		rcx, rcx
;	xor		r8, r8
;	mov		r8d, eax
;	xor		eax, eax
;	jmp		loop
;
;loop:
;	xor		r9, r9
;	mov		r9b, r8b
;	call	getInvSboxEntry ;update eax with substitue words
;	shr		r8d, 8
;
;	inc		rcx
;	cmp		rcx, Nb
;	jl		loop
;
;	pop		rcx
;	ret
;
;getInvSboxEntry:
;	;instruction: mov	eax, [rdx + r9b * 16 + r10b]
;	mov		al, [rdx + r9]
;	cmp		rcx, Nb - 1
;	jne		shiftLeft
;	ret
;
;shiftLeft:
;	shl 	eax, 8
;	ret
;
