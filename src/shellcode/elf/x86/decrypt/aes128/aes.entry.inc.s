; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/11 23:03:43 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits 32
default rel

_woody_decrypt_aes_entry:
	call	keyExpansion

	mov	ecx, 0x10
	call	decrypt_loop

	ret

decrypt_loop:
	push	ecx
	call	decrypt
	pop	ecx
	add	edi, 0x10

	add	ecx, 0x10
	cmp	ecx, edx
	jl	decrypt_loop

	ret

%include "elf/x86/decrypt/aes128/aes-128.inc.s"
%include "elf/x86/decrypt/aes128/keyExpansion.inc.s"
