invSubBytes:
	push	edx
	lea		edx, [rel inv_mixcolumns_matrix]

	push	ecx
	push	esi
	push	ebx

	xor		ecx, ecx
	call	invSubBytes_loop

	pop		ebx
	pop		esi
	pop		ecx
	pop		edx
	ret

invSubBytes_loop:
	mov		eax, [edi + 4 * ecx]
	call	invSubBytes_32bits_words
	bswap	eax
	mov		[edi + 4 * ecx], eax

	inc		ecx
	cmp		ecx, Nb
	jl		invSubBytes_loop

	ret

invSubBytes_32bits_words:
	push	ecx
	xor		ecx, ecx
	xor		esi, esi
	mov		esi, eax
	xor		eax, eax
	jmp		loop

loop:
	xor		ebx, ebx
	mov		bx, si
	and		bx, 0x00FF
	call	getInvSboxEntry ;update eax with substitue words
	shr		esi, 8

	inc		ecx
	cmp		ecx, Nb
	jl		loop

	pop		ecx
	ret

getInvSboxEntry:
	mov		al, [edx + ebx]
	cmp		ecx, Nb - 1
	jne		shiftLeft
	ret

shiftLeft:
	shl 	eax, 8
	ret

