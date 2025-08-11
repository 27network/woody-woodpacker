; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/11 23:02:07 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits 64
default rel

_woody_decrypt_aes_entry:
	call	get_ciphertext_size_modulo_16
	call	initialize_registers
	call	keyExpansion

	xor		rcx, rcx
	cmp		r14, 0
	jne		decrypt_loop

	ret

decrypt_loop:
	push	rcx
	xor		rcx, rcx
	call	decrypt
	add		rdi, 0x10

	pop		rcx
	add		rcx, 0x10
	cmp		rcx, r14
	jl		decrypt_loop

	ret

get_ciphertext_size_modulo_16:
	cmp		rdx, 16
	jl		no_decryption
	jg		get_modulo_16
	mov		r14, rdx

	ret

no_decryption:
	mov		r14, 0

	ret

get_modulo_16:
	test	rdx, 0x0F
	jnz		not_modulo_16
	mov		r14, rdx

	ret

not_modulo_16:
	sub		rdx, 0x10
	mov		r14, rdx

	ret

%include "elf/x86_64/decrypt/aes128/aes-128.inc.s"
%include "elf/x86_64/decrypt/aes128/keyExpansion.inc.s"
