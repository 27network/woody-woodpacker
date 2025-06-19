; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.entry.s                                 :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/03/30 15:34:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/19 17:40:21 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel
global _woody_start 

_woody_start:
	; argc = [RBP + 8]
	; argv = [RBP + 16]
	; envp = [RBP + 24]

	; 1. call _woody_decrypt
	; lea RDI, [rel segments_content]
	; add RDI, [rel segments_write_offset]

	; 2. call _woody_decompress

	; 3. call _woody_loader to execute the provided payload
	call _woody_loader
	
	; A bit of cleanup
	xor RDI, RDI
	xor RSI, RSI
	xor RDX, RDX
	xor RCX, RCX
	xor RBP, RBP
	xor SYS_NUM, SYS_NUM
	xor SYS_ARG0, SYS_ARG0
	xor SYS_ARG1, SYS_ARG1
	xor SYS_ARG2, SYS_ARG2
	xor SYS_ARG3, SYS_ARG3
	xor SYS_ARG4, SYS_ARG4
	xor SYS_ARG5, SYS_ARG5

	; 4. jump to _start
	lea RAX, [rel _woody_start] ; get the address of _woody_start
	mov RDI, [rel start_offset] ; get the offset to _start
	add RAX, RDI ; add the offset to _start
	xor RDI, RDI

	jmp RAX ; yeet! 


%include "elf/common/entry/loader.inc.s"

; This is where program-set variables are stored,
; i.e. where woody will store addresses, offsets, payloads, etc.
%include "elf/common/entry/variables.inc.s"
