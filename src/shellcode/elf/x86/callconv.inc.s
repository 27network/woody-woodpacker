; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    callconv.inc.s                                     :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:23:16 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/17 17:11:16 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; This file contains the call convention for the x86 architecture.
; It is used by the entrypoint.inc.s and payload.inc.s files to ensure
; compatibility with x86 and x86_64 compilation targets.

%define BITS 32

; Simple registers
%define RAX eax
%define RDI edi
%define RSI esi
%define RDX edx
%define RBP ebp
%define RSP esp

; Syscall related registries
%define SYS_NUM eax
%define SYS_ARG0 ebx
%define SYS_ARG1 ecx
%define SYS_ARG2 edx
%define SYS_ARG3 esi
%define SYS_ARG4 edi
%define SYS_ARG5 ebp
%define SYSCALL int 0x80

; Syscall numbers
%include "elf/x86/syscalls.inc.s"

; NASM declarations
%define DECLARE_SIZE dd
