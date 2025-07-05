; Registers use:
;
; rdx = mixcolumns_matrix
; rdi = plaintext


invMixColumns:
	call	update_plaintext_in_memory
	push	rcx
	xor	rcx, rcx
	jmp	invMixColumns_loop

invMixColumns_loop:
	xor	rax, rax
	xor	r9, r9
	xor 	r11, r11
	call	column_multiplication_loop
	bswap	eax
	mov	[rdi + 4 * rcx], eax

	inc	rcx
	cmp	rcx, Nb
	jl	invMixColumns_loop

	pop	rcx
	call	update_xmm0
	ret

column_multiplication_loop:
	push	rax
	push	rcx
	shl	rcx, 2
	add	rcx, r11
	call	multiplication
	pop	rcx
	pop	r8
	xor	al, r8b

	inc	r9
	inc	r11

	mov	r8, r9
	and	r8, 3
	jnz	column_multiplication_loop

	shl	rax, 8
	xor	r11, r11
	cmp	r9, Nb * 4
	jl	column_multiplication_loop

	shr	rax, 8
	ret

multiplication:
	mov	al, [rdi + rcx]
	cmp	byte [rdx + r9], 0x09
	je	multiplication_by_0x09
	cmp	byte [rdx + r9], 0x0b
	je	multiplication_by_0x0b
	cmp	byte [rdx + r9], 0x0d
	je	multiplication_by_0x0d
	cmp	byte [rdx + r9], 0x0e
	je	multiplication_by_0x0e

multiplication_by_0x09:
	mov	r8b, al
	call	multiplication_by_2
	call	multiplication_by_2
	call	multiplication_by_2
	xor	al, r8b

	ret

multiplication_by_0x0b:
	mov	r8b, al
	call	multiplication_by_2
	call	multiplication_by_2
	xor	al, r8b
	call	multiplication_by_2
	xor	al, r8b

	ret

multiplication_by_0x0d:
	mov	r8b, al
	call	multiplication_by_2
	xor	al, r8b
	call	multiplication_by_2
	call	multiplication_by_2
	xor	al, r8b

	ret

multiplication_by_0x0e:
	mov	r8b, al
	call	multiplication_by_2
	xor	al, r8b
	call	multiplication_by_2
	xor	al, r8b
	call	multiplication_by_2

	ret

multiplication_by_2:
	mov	r10b, al
	shl	al, 1
	and	r10b, 0b10000000
	jnz	modulo_reduction
	ret

modulo_reduction:
	xor	al, irreducible_polynomial
	ret

update_plaintext_in_memory:
	test	rdi, 0xF
	jz	aligned_address_to_memory
	jmp	unaligned_address_to_memory

aligned_address_to_memory:
	movdqa	[rdi], xmm0
	ret

unaligned_address_to_memory:
	movdqu	[rdi], xmm0
	ret

update_xmm0:
	test	rdi, 0xF
	jz	aligned_address_to_xmm0
	jmp	unaligned_address_to_xmm0

aligned_address_to_xmm0:
	movdqa	xmm0, [rdi]
	ret

unaligned_address_to_xmm0:
	movdqu	xmm0, [rdi]
	ret
