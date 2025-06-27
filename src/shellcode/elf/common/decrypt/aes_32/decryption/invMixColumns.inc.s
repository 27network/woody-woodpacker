invMixColumns:
	lea		ebx, [rel inv_mixcolumns_matrix]

	push	edx
	push	esi
	push	ecx

	xor		esi, esi
	xor		eax, eax
	xor		ecx, ecx
	call	invMixColumns_loop

	pop		ecx
	pop		esi
	pop		edx
	ret

invMixColumns_loop:
	push	ecx

	test	ecx, 0x3
	jz		set_esi_to_zero
	jmp		continue_loop

continue_loop:
	mov		edx, [ebx + esi]

	push	esi
	call	get_state_index
	xor		ecx, ecx
	call 	multiplication_loop
	pop		esi

	pop		ecx
	call	write_to_memory_if_modulo_4
	add		esi, 4

	inc		ecx
	cmp		ecx, Nb * 4
	jl		invMixColumns_loop

	ret

multiplication_loop:
	push	edx
	push	eax
	call	multiplication
	pop		edx
	xor		al, dl

	pop		edx
	shr		edx, 8
	inc		ecx
	cmp		ecx, Nb
	jl		multiplication_loop

	ret

multiplication:
	cmp		dl, 0x09
	je		multiplication_by_0x09
	cmp		dl, 0x0b
	je		multiplication_by_0x0b
	cmp		dl, 0x0d
	je		multiplication_by_0x0d
	cmp		dl, 0x0e
	je		multiplication_by_0x0e

multiplication_by_0x09:
	add		esi, ecx
	mov		al, [edi + esi]
	sub		esi, ecx

	mov		dl, al
	push	edx
	call	multiplication_by_2
	call	multiplication_by_2
	call	multiplication_by_2
	pop		edx
	xor		al, dl

	ret

multiplication_by_0x0b:
	add		esi, ecx
	mov		al, [edi + esi]
	sub		esi, ecx

	mov		dl, al
	push	edx
	call	multiplication_by_2
	call	multiplication_by_2
	pop		edx

	xor		al, dl
	push	edx
	call	multiplication_by_2
	pop		edx
	xor		al, dl

	ret

multiplication_by_0x0d:
	add		esi, ecx
	mov		al, [edi + esi]
	sub		esi, ecx

	mov		dl, al
	push	edx
	call	multiplication_by_2
	pop		edx

	xor		al, dl
	push	edx
	call	multiplication_by_2
	call	multiplication_by_2
	pop		edx
	xor		al, dl

	ret

multiplication_by_0x0e:
	add		esi, ecx
	mov		al, [edi + esi]
	sub		esi, ecx

	mov		dl, al
	push	edx
	call	multiplication_by_2
	pop		edx

	xor		al, dl
	push	edx
	call	multiplication_by_2
	pop		edx

	xor		al, dl
	call	multiplication_by_2

	ret

multiplication_by_2:
	mov		dl, al
	shl		al, 1
	and		dl, 0b10000000
	jnz		modulo_reduction

	ret

write_to_memory_if_modulo_4:
	inc		ecx
	test	ecx, 0x3
	jz		write_to_memory
	dec		ecx
	shl		eax, 8

	ret

write_to_memory:
	sub		ecx, 4
	bswap	eax
	mov		[edi + ecx], eax
	xor		eax, eax
	add		ecx, 3

	ret

modulo_reduction:
	xor		al, irreducible_polynomial
	ret

get_state_index:
	cmp		ecx, 4
	jl		set_esi_to_0
	cmp		ecx, 8
	jl		set_esi_to_4
	cmp		ecx, 12
	jl		set_esi_to_8
	jmp		set_esi_to_12

set_esi_to_0:
	xor		esi, esi
	ret

set_esi_to_4:
	mov		esi, 4
	ret

set_esi_to_8:
	mov		esi, 8
	ret

set_esi_to_12:
	mov		esi, 12
	ret

set_esi_to_zero:
	xor		esi, esi
	jmp		continue_loop
