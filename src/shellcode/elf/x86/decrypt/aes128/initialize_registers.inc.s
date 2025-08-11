; Registers use:
;
; edi	-> pointer to ciphertext
; esi	-> encryption key (to be expanded)
; ebx	-> inv_mixcolumns_matrix

initialize_registers:
	call	get_rel_s_box
	call	get_rel_inv_mixcolumns_matrix
	xor		eax, eax
	xor		ecx, ecx

	ret

get_rel_s_box:
	call	addr_s_box
addr_s_box:
	pop		edx
	add		edx, s_box - addr_s_box

	ret

get_rel_inv_mixcolumns_matrix:
	call	addr_inv_mixcolumns_matrix
addr_inv_mixcolumns_matrix:
	pop		ebx
	add		ebx, inv_mixcolumns_matrix - addr_inv_mixcolumns_matrix

	ret
