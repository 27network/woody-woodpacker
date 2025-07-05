; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry_64.inc.s                                 :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/04 17:39:59 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%include "elf/common/decrypt/aes_64/aes-128.inc.s"
%include "elf/common/decrypt/aes_64/keyExpansion.inc.s"

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
