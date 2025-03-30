; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/02/20 17:55:33 by kiroussa          #+#    #+#              ;
;    Updated: 2025/03/30 15:38:30 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%define BITS 32
%define RAX eax
%define RDI edi
%define RSI esi
%define RDX edx
%define SYSCALL int 0x80
%include "src/shellcode/elf/common/entrypoint.inc.s"
