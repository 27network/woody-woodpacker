addRoundKey:
	push	eax
	push	edx
	push	ebx

	mov	edx, ecx
	xor	ecx, ecx
	jmp	addRoundKey_loop

addRoundKey_loop:
	push	edx	; edx = old ecx = round number
	imul	edx, Nb * 4
	shl	ecx, 2
	add	edx, ecx
	shr	ecx, 2
	mov	ebx, [esi + edx]
	mov	eax, [edi + ecx * 4]
	xor	eax, ebx
	mov	[edi + ecx * 4], eax
	pop 	edx

	inc	ecx
	cmp	ecx, Nb
	jl	addRoundKey_loop

	mov	ecx, edx

	pop	ebx
	pop	edx
	pop	eax
	ret
