keyExpansion:
	xor		rcx, rcx
	add		rcx, 4
	mov		eax, [rsi + 12]
	jmp		expansion_loop

expansion_loop:
	test	cl, 3	;calculate rcx % 4
	jz		g_vector
	mov		r8d, [rsi + 4 * (rcx - 4)]
	xor		eax, r8d
	mov		dword [rsi + 4 * rcx], eax

	inc		rcx
	cmp		rcx, Nb * (Nr + 1)
	jl		expansion_loop
	ret

g_vector:
	bswap	eax
	call	shiftWord ;on last rsi-expanded word (eax)
	call	subBytes_32bits_words ;on last rsi-expanded word (eax)
	xor		eax, [rbx + rcx - 4] ;Rcon
	mov		r8d, [rsi + 4 * (rcx - 4)]
	bswap	eax
	xor		eax, r8d
	mov		dword [rsi + 4 * rcx], eax
	inc		rcx
	cmp		rcx, Nb * (Nr + 1)
	jl		expansion_loop
	ret

shiftWord:
	mov		r8d, eax
	shl		eax, 8
	shr		r8d, 24
	add		al, r8b

	ret

subBytes_32bits_words:
	vpcmpeqd		ymm3, ymm3, ymm3
	movd			xmm4, eax
	vpmovzxbd		ymm4, xmm4
	push			rax
	lea				rax, [rel s_box]
	vpgatherdd		ymm5, [rax + ymm4 * 4], ymm3
	pop				rax
	vextracti128	xmm4, ymm5, 0
	pshufb			xmm4, xmm1
	movd			eax, xmm4

	ret
