; Registers use:
;
; xmm0 -> ciphertext
; xmm1 -> vector_depacking_mask
; xmm2 -> inv_shift_mask
; rdi  -> pointer to ciphertext
; rsi  -> encryption key (expanded)
; rdx  -> inv_mixcolumns_matrix

initialize_registers:
	lea		rax, [rel vector_depacking_mask]
	movdqa	xmm1, [rax]
	lea		rax, [rel inv_shift_mask]
	movdqa	xmm2, [rax]
	lea		rsi, [rel OFFSET_ENCRYPTION_KEY]
	lea		rdx, [rel inv_mixcolumns_matrix]
	lea		rdi, [rel OFFSET_ENCRYPTED_CODE]
	test	rdi, 0xF
	jz		get_aligned_memory
	jmp		get_unaligned_memory

get_aligned_memory:
	movdqa	oword xmm0, [rdi]
	ret

get_unaligned_memory:
	movdqu	oword xmm0, [rdi]
	ret
