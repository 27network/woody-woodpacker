; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/26 16:40:26 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel

%include "elf/common/decrypt/aes_64/aes-128.inc.s"

_woody_decrypt_aes_x64:
	call	keyExpansion
	call	initialize_registers

	mov		rcx, 0x10
	call	decrypt_loop

	ret

decrypt_loop:
	call	decrypt
	add		rdi, 0x10

	add		rcx, 0x10
	cmp		rcx, rdx
	jl		decrypt_loop

	ret

keyExpansion:
	xor		rcx, rcx
	add		rcx, 4
	mov		eax, [rsi + 12]
	jmp		expansion_loop

expansion_loop:
	test	cl, 3	;calculate rcx % 4
	jz		g_vector
	mov		r8d, [rsi + 4 * (rcx - 4)]
	xor		eax, r8d
	mov		dword [rsi + 4 * rcx], eax

	inc		rcx
	cmp		rcx, Nb * (Nr + 1)
	jl		expansion_loop
	ret

g_vector:
	bswap	eax
	call	shiftWord ;on last rsi-expanded word (eax)
	call	subBytes_32bits_words ;on last rsi-expanded word (eax)
	xor		eax, [rbx + rcx - 4] ;Rcon
	mov		r8d, [rsi + 4 * (rcx - 4)]
	bswap	eax
	xor		eax, r8d
	mov		dword [rsi + 4 * rcx], eax
	inc		rcx
	cmp		rcx, Nb * (Nr + 1)
	jl		expansion_loop
	ret

shiftWord:
	mov		r8d, eax
	shl		eax, 8
	shr		r8d, 24
	add		al, r8b

	ret

subBytes_32bits_words:
	vpcmpeqd		ymm3, ymm3, ymm3
	movd			xmm4, eax
	vpmovzxbd		ymm4, xmm4
	push			rax
	lea				rax, [rel inv_s_box]
	vpgatherdd		ymm5, [rax + ymm4 * 4], ymm3
	pop				rax
	vextracti128	xmm4, ymm5, 0
	pshufb			xmm4, xmm1
	movd			eax, xmm4

	ret
