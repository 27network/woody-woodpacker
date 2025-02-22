; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    payload.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/02/20 17:48:45 by kiroussa          #+#    #+#              ;
;    Updated: 2025/02/20 18:03:00 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits 64
default rel
global _wentry 

_wentry:
    ; write(1, hello, 14)
    mov rax, 1
    mov rdi, 1
	lea rsi, [rel hello]
    mov rdx, 14
    syscall

    ; exit(0)
    mov rax, 60
    xor rdi, rdi
    syscall

hello:
    db "Hello, Woody!\n", 0
