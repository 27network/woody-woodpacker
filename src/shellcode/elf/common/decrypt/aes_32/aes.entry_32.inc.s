; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry_32.inc.s                                 :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/04 17:39:59 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%include "elf/common/decrypt/aes_32/aes-128.inc.s"
%include "elf/common/decrypt/aes_32/keyExpansion.inc.s"

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
