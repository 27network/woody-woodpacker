invShiftRows:
	push	ecx
	xor		ecx, ecx
	jmp		invShiftRows_loop

invShiftRows_loop:
	call	invShiftRows_32bits_words

	inc		ecx
	cmp		ecx, Nb
	jl		invShiftRows_loop

	pop		ecx
	ret

invShiftRows_32bits_words:
	;get row state
	mov		al, [edi + ecx]
	shl		eax, 8
	mov		al, [edi + 4 + ecx]
	shl		eax, 8
	mov		al, [edi + 8 + ecx]
	shl		eax, 8
	mov		al, [edi + 12 + ecx]

	jmp 	update_ecx_row_state

update_ecx_row_state:
	cmp		ecx, 1
	je		shift_second_row
	cmp		ecx, 2
	je		shift_third_row
	cmp		ecx, 3
	je		shift_fourth_row

	ret

shift_second_row:
	mov		[edi + ecx], al
	shr		eax, 8
	mov		[edi + ecx + 12], al
	shr		eax, 8
	mov		[edi + ecx + 8], al
	shr		eax, 8
	mov		[edi + ecx + 4], al

	ret

shift_third_row:
	mov		[edi + ecx + 4], al
	shr		eax, 8
	mov		[edi + ecx], al
	shr		eax, 8
	mov		[edi + ecx + 12], al
	shr		eax, 8
	mov		[edi + ecx + 8], al

	ret

shift_fourth_row:
	mov		[edi + ecx + 8], al
	shr		eax, 8
	mov		[edi + ecx + 4], al
	shr		eax, 8
	mov		[edi + ecx], al
	shr		eax, 8
	mov		[edi + ecx + 12], al

	ret
