global	mixColumns


; Registers use:
;
; rdx = mixcolumns_matrix
; rdi = plaintext


mixColumns:
	call	update_plaintext_in_memory
	push	rcx
	xor		rcx, rcx
	jmp		mixColumns_loop

mixColumns_loop:
	xor		rax, rax
	xor		r9, r9
	xor 	r11, r11
	call	column_multiplication_loop
	bswap	eax
	mov		[rdi + 4 * rcx], eax

	inc		rcx
	cmp		rcx, Nb
	jl		mixColumns_loop

	pop		rcx
	call	update_xmm0
	ret

column_multiplication_loop:
	push	rax
	push	rcx
	shl		rcx, 2
	add		rcx, r11
	call	multiplication
	pop		rcx
	pop		r8
	xor		al, r8b

	inc		r9
	inc		r11

	mov		r8, r9
	and		r8, 3
	jnz		column_multiplication_loop

	shl		rax, 8
	xor		r11, r11
	cmp		r9, Nb * 4
	jl		column_multiplication_loop

	shr		rax, 8
	ret

multiplication:
	cmp		byte [rdx + r9], 2
	je		multiplication_by_2
	jg		multiplication_by_3
	;multiplication by 1
	mov		al, [rdi + rcx]
	ret

multiplication_by_2:
	mov		al, [rdi + rcx]
	mov		r10b, al
	shl		al, 1
	and		r10b, 0b10000000
	jnz		modulo_reduction
	ret

modulo_reduction:
	; TODO(jsauvain): Ça met un warning, wat?
	xor		al, irreducible_polynomial
	ret

multiplication_by_3:
	mov		r10b, [rdi + rcx]
	push	r10
	call	multiplication_by_2
	pop		r10
	xor		al, r10b
	ret

update_plaintext_in_memory:
	test	rdi, 0xF
	jz		aligned_address_to_memory
	jmp		unaligned_address_to_memory

aligned_address_to_memory:
	movdqa	[rdi], xmm0
	ret

unaligned_address_to_memory:
	movdqu	[rdi], xmm0
	ret

update_xmm0:
	test	rdi, 0xF
	jz		aligned_address_to_xmm0
	jmp		unaligned_address_to_xmm0

aligned_address_to_xmm0:
	movdqa	xmm0, [rdi]
	ret

unaligned_address_to_xmm0:
	movdqu	xmm0, [rdi]
	ret
