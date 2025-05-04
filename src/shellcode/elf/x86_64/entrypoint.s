; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/02/20 17:48:45 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/02 16:22:25 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%define BITS 64
%define RAX rax
%define RDI rdi
%define RSI rsi
%define RDX rdx
%define SYSCALL syscall
%include "src/shellcode/elf/common/entrypoint.inc.s"

section .data
# db payload "#!/bin/sh\necho hi\n"
db payload 0xC3
db payload_size equ $ - payload
