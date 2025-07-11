global	first_round_transformation
global	middle_round_transformations
global	last_round_transformations

extern shiftRows
extern mixColumns

first_round_transformation:
	xor		rcx, rcx
	call	addRoundKey
	inc		rcx

	ret

middle_round_transformations:
	call	subBytes
	call	shiftRows
	call	mixColumns
	call	addRoundKey

	inc		rcx
	cmp		rcx, Nr
	jl		middle_round_transformations

	ret

last_round_transformations:
	call	subBytes
	call	shiftRows
	call	addRoundKey

	ret
