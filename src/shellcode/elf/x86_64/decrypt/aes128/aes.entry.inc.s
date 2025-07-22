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
	call	keyExpansion
	call	initialize_registers

	mov	rcx, 0x10
	call	decrypt_loop

	ret

decrypt_loop:
	call	decrypt
	add	rdi, 0x10

	add	rcx, 0x10
	cmp	rcx, rdx
	jl	decrypt_loop

	ret

%include "elf/x86_64/decrypt/aes128/aes-128.inc.s"
%include "elf/x86_64/decrypt/aes128/keyExpansion.inc.s"
