; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    none.entry.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/06/20 14:44:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/22 02:51:51 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel

; Even if we don't use a decompression method, we still need to write back the
; segments content to the ELF's original memory page.
_woody_decompress_none:
	xor RAX, RAX
.loop:
	mov cl, byte [RDI + RAX]
	mov byte [RDX + RAX], cl 
	inc RAX
	cmp RAX, RSI
	je .end
	jmp .loop
.end:
	ret
