; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.entry.s                                 :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/03/30 15:34:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/20 14:23:31 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel

_woody_start_base:
	; 0. setup base for relative addressing
	call _woody_start
_woody_start:
	; **NOTE**: RAX is used for relative addressing, since 32bit assembly
	;			doesn't support RIP-relative addressing.  We just use RAX
	;           as a temporary register to offset from.
	pop RAX ; RAX = _woody_start

	; 1. call _woody_decrypt_aes
	; lea RDI, [rel segments_content]
	; add RDI, [rel segments_write_offset]

	; 2. call _woody_decompress_smlz

	; 3. call _woody_loader to execute the provided payload
	call _woody_loader
	
	; A bit of cleanup (do NOT touch RAX, we still need it)
	xor RDI, RDI
	xor RSI, RSI
	xor RDX, RDX
	xor RCX, RCX
	xor RBP, RBP
	xor SYS_ARG0, SYS_ARG0
	xor SYS_ARG1, SYS_ARG1
	xor SYS_ARG2, SYS_ARG2
	xor SYS_ARG3, SYS_ARG3
	xor SYS_ARG4, SYS_ARG4
	xor SYS_ARG5, SYS_ARG5

	; 4. jump to _start
	lea RSI, [RAX + OFFSET_WOODY_START_BASE] ; get the address of _woody_start
	mov RDI, [RAX + OFFSET_START_OFFSET] ; get the offset to _start
	add RSI, RDI ; add the offset to _start
	mov RAX, RSI ; overwrite RAX since we don't need it anymore
	xor RDI, RDI ; cleanup last registers
	xor RSI, RSI

	jmp RAX ; yeet! 


%include "elf/common/entry/loader.inc.s"

; This is where program-set variables are stored,
; i.e. where woody will store addresses, offsets, payloads, etc.
%include "elf/common/entry/variables.inc.s"
