inv_first_round_transformation:
	mov		rcx, Nr
	call	addRoundKey
	call	invShiftRows
	call	invSubBytes
	dec		rcx

	ret

inv_middle_round_transformations:
	call	addRoundKey
	call	invMixColumns
	call	invShiftRows
	call	invSubBytes

	dec		rcx
	cmp		rcx, 0
	jg		inv_middle_round_transformations

	ret

inv_last_round_transformations:
	call	addRoundKey
	ret
