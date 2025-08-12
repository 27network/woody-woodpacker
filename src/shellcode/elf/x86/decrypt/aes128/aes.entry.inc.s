; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/22 15:30:10 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits 32
default rel

_woody_decrypt_aes_entry:
	call	get_ciphertext_size_modulo_16
	push	ecx
	call	initialize_registers
	call	keyExpansion
	pop		ecx

	call	get_rel_inv_s_box

	cmp		ecx, 0
	jne		decrypt_loop

	ret

decrypt_loop:
	push	ecx
	xor		ecx, ecx
	call	decrypt
	add		edi, 0x10

	pop		ecx
	cmp		ecx, 0x10
	jl		exit

	sub		ecx, 0x10
	cmp		ecx, 0
	jne		decrypt_loop

	ret

exit:
	ret

get_ciphertext_size_modulo_16:
	cmp		edx, 16
	jl		no_decryption
	jg		get_modulo_16
	mov		ecx, edx

	ret

no_decryption:
	mov		ecx, 0

	ret

get_modulo_16:
	test	edx, 0x0F
	jnz		not_modulo_16
	mov		ecx, edx

	ret

not_modulo_16:
	sub		edx, 0x10
	mov		ecx, edx

	ret

get_rel_inv_s_box:
	call	addr_inv_s_box
addr_inv_s_box:
	pop		edx
	add		edx, inv_s_box - addr_inv_s_box

	ret

%include "elf/x86/decrypt/aes128/aes-128.inc.s"
%include "elf/x86/decrypt/aes128/keyExpansion.inc.s"
