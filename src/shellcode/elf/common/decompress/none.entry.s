; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    none.entry.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/06/20 14:44:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/18 12:33:04 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel

_payload:
	call _payload_entry

message: db "lol xd", 10
message_len: equ $-message

_payload_entry:
	mov SYS_ARG0, 1 ; stdout
	pop SYS_ARG1 ; message
	mov SYS_ARG2, message_len ; message_len
	; write(1, "....WOODY....\n", 14)
	mov SYS_NUM, SYS_WRITE
	SYSCALL

	; The shellcode payload should always return
	ret
