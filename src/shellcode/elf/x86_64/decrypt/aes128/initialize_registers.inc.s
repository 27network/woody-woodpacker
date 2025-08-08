; Registers use:
;
; xmm0				-> ciphertext
; xmm1				-> vector_depacking_mask
; xmm2				-> inv_shift_mask
; rdi (argument)	-> pointer to ciphertext
; rsi (argument)	-> encryption key (expanded)
; rbx				-> inv_mixcolumns_matrix

initialize_registers:
	lea		rax, [rel vector_depacking_mask]
	movdqu	xmm1, [rax]
	lea		rax, [rel inv_shift_mask]
	movdqu	xmm2, [rax]
	lea		rbx, [rel inv_mixcolumns_matrix]
	test	rdi, 0xF
	jz		get_aligned_memory
	jmp		get_unaligned_memory

get_aligned_memory:
	movdqa	oword xmm0, [rdi]
	ret

get_unaligned_memory:
	movdqu	oword xmm0, [rdi]
	ret
