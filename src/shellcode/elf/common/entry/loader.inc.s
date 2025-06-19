; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    loader.inc.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:11:06 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/19 17:45:16 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Variables (see variables.inc.s):
;   - payload
;   - payload_size

_woody_loader:
	mov RSI, [rel user_payload_size]
	cmp RSI, 0
	je .Lend

 	lea RAX, [rel segments_content]
 	add RAX, [rel segments_content_size] 

 	cmp RSI, 0x4 ; if rsi is less than 4, it's not a valid ELF
 	jl .Lshellcode

 	cmp BYTE [RAX], 0x7F ; elf magic
 	jne .Lshellcode
 	cmp BYTE [RAX + 1], 0x45 ; E
 	jne .Lshellcode
 	cmp BYTE [RAX + 2], 0x4C ; L
 	jne .Lshellcode
 	cmp BYTE [RAX + 3], 0x46 ; F
 	jne .Lshellcode

 	; TODO: Implement

 	jmp .Lend

 .Lshellcode:
  	call RAX

.Lend:
	ret
