keyExpansion:
	push	edi
	push	ebx

	add	ecx, 4
	mov	eax, [esi + 12]
	xor	edi, edi
	jmp	expansion_loop

expansion_loop:
	test	cl, 3	;calculate ecx % 4
	jz	g_vector

	push	edx
	mov	edx, [esi + 4 * (ecx - 4)]
	xor	eax, edx
	pop	edx
	mov	[esi + 4 * ecx], eax

	inc	ecx
	cmp	ecx, Nb * (Nr + 1)
	jl	expansion_loop

	pop	ebx
	pop	edi
	ret

g_vector:
	bswap	eax
	call	shiftWord ;on last key-expanded word (eax)
	push	edi
	call	subBytes_32bits_words ;on last key-expanded word (eax)
	pop	edi
	call	Rcon
	push	edx
	mov	edx, [esi + 4 * (ecx - 4)]
	xor	eax, edx
	pop	edx
	mov	[esi + 4 * ecx], eax
	inc	ecx
	cmp	ecx, Nb * (Nr + 1)
	jl	expansion_loop
	ret

subBytes_32bits_words:
	push    esi
	push    ebx
	push    ecx

	xor	ecx, ecx
	xor	esi, esi

	mov	esi, eax
	xor	eax, eax
	jmp	loop

shiftWord:
	push	edx
	mov	edx, eax
	shl	eax, 8
	shr	edx, 24
	add	al, dl
	pop	edx

	ret

Rcon:
	push	ecx
	push	edx

	xor	ecx, ecx
	mov	edx, Rcon_base
	jmp	Rcon_loop

Rcon_loop:
	cmp	ecx, edi
	jge	xor_operation

	mov	ebx, edx
	shl	dl, 1
	and	ebx, 0b10000000
	jnz	.modulo_reduction

	inc	ecx
	jmp	Rcon_loop

.modulo_reduction:
	xor	edx, 0x1b

	inc	ecx
	jmp	Rcon_loop

xor_operation:
	xor	eax, edx
	inc	edi

	pop	edx
	pop	ecx
	ret
