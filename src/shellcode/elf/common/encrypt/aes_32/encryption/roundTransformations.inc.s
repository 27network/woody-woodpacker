first_round_transformation:
	xor	ecx, ecx
	call	addRoundKey
	inc	ecx

	ret

middle_round_transformations:
	call	subBytes
	call	shiftRows
	call	mixColumns
	call	addRoundKey

	inc	ecx
	cmp	ecx, Nr
	jl	middle_round_transformations

	ret

last_round_transformations:
	call	subBytes
	call	shiftRows
	call	addRoundKey

	ret
