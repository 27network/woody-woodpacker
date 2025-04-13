; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.inc.s                                   :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/03/30 15:34:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/04/08 13:37:05 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel
global _wentry 

_wentry:
    ; write(1, hello, 14)
    mov RAX, 1
    mov RDI, 1
	lea RSI, [rel hello]
    mov RDX, 14
    SYSCALL

    ; exit(0)
    mov RAX, 60
    xor RDI, RDI
	SYSCALL

hello:
    db hello, "Hello, Woody!\n", 0

