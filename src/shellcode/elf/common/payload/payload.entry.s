; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    payload.entry.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:12:35 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/26 21:33:39 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; This file is the default payload for woody-woodpacker,
; it is a simple shellcode that will print "....WOODY....\n" to stdout.

; It's compiled separately from the rest of the injected code (entrypoint.s)
; since this payload is self-contained and swappable via woody.

bits BITS
default rel

_payload:
	; write(1, "....WOODY....\n", 14)
	mov SYS_NUM, SYS_WRITE
	mov SYS_ARG0, 1 ; stdout
	mov SYS_ARG1, [rel message]
	mov SYS_ARG2, message_len
	SYSCALL

	; The shellcode payload should always return
	ret

message: db "....WOODY....\n", 0
message_len: equ $-message
