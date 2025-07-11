; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    callconv.inc.s                                     :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:23:16 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/17 19:39:23 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; This file contains the call convention for the x86_64 architecture.
; It is used by the entrypoint.inc.s and payload.inc.s files to ensure
; compatibility with x86 and x86_64 compilation targets.

%define BITS 64

; Simple registers
%define RAX rax
%define RDI rdi
%define RSI rsi
%define RDX rdx
%define RCX rcx
%define RBX rbx

%define RBP rbp
%define RSP rsp

; Syscall related registries
%define SYS_NUM rax
%define SYS_ARG0 rdi
%define SYS_ARG1 rsi
%define SYS_ARG2 rdx
%define SYS_ARG3 r10
%define SYS_ARG4 r8
%define SYS_ARG5 r9
%define SYSCALL syscall

; Syscall numbers
%include "elf/x86_64/syscalls.inc.s"

; NASM declarations
%define DECLARE_SIZE dq
