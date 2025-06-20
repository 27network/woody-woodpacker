; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    loader.inc.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:11:06 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/20 14:10:08 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Variables (see variables.inc.s):
;   - payload
;   - payload_size

_woody_loader:
	mov RSI, [RAX + OFFSET_USER_PAYLOAD_SIZE]
	cmp RSI, 0
	je .Lend

 	lea RDX, [RAX + OFFSET_SEGMENTS_CONTENT]
 	add RDX, [RAX + OFFSET_SEGMENTS_CONTENT_SIZE]

 	cmp RSI, 0x4 ; if rsi is less than 4, it's not a valid ELF
 	jl .Lshellcode

 	cmp BYTE [RDX], 0x7F ; elf magic
 	jne .Lshellcode
 	cmp BYTE [RDX + 1], 0x45 ; E
 	jne .Lshellcode
 	cmp BYTE [RDX + 2], 0x4C ; L
 	jne .Lshellcode
 	cmp BYTE [RDX + 3], 0x46 ; F
 	jne .Lshellcode

 	; TODO: Implement

 	jmp .Lend

 .Lshellcode:
	push RAX
  	call RDX
	pop RAX

.Lend:
	ret
