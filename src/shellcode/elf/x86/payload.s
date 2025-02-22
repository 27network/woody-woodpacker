; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    payload.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/02/20 17:55:33 by kiroussa          #+#    #+#              ;
;    Updated: 2025/02/20 17:57:34 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

global _wentry 

_wentry:
    ; write(1, hello, 14)
    mov eax, 1
    mov ebx, 1
	lea ecx, [hello]
    mov edx, 14
    int 0x80

    ; exit(0)
    mov eax, 60
    xor ebx, ebx
    int 0x80

hello:
    db "Hello, Woody!\n", 0
