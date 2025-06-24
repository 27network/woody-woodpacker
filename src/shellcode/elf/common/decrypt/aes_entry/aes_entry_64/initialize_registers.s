; ymm1/masks	-> Lower part = shiftRows_Mask
;				-> Higher part = vector_depacking_Mask

initialize_registers:
	lea		rax, [rel vector_depacking_mask]
	movdqa	xmm1, [rax]
	lea		rax, [rel shift_masks]
	vmovdqa	ymm2, [rax]
	lea		rsi, [rel key]
	lea		rdx, [rel mixcolumns_matrix]
	lea		rbx, [rel Rcon_table]
	lea		rax, [rdi]
	test	rax, 0xF
	jz		get_aligned_memory
	jmp		get_unaligned_memory

get_aligned_memory:
	movdqa	oword xmm0, [rax]
	ret

get_unaligned_memory:
	movdqu	oword xmm0, [rax]
	ret
