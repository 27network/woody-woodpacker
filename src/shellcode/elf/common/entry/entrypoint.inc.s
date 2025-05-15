; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.inc.s                                   :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/03/30 15:34:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/15 18:16:24 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel
global _woody_entry 

_woody_entry:
	; argc = [RBP + 8]
	; argv = [RBP + 16]
	; envp = [RBP + 24]

	; 1. call _woody_loader to execute the provided payload
	; call [rel _woody_loader]

	; 2. call _woody_decrypt
	; lea RDI, [rel segments_content]
	; add RDI, [rel segments_write_offset]

	; 3. call _woody_decompress
	
	; 4. jump to _start

	mov RDI, [rel start_offset]
	sub RDI, 16
	call get_rip
get_rip:
	pop RAX
	add RAX, RDI 
	jmp RAX ; yeet!

; %include "elf/common/loader.inc.s"
; %include "decrypt.inc.s"
; %include "decompress.inc.s"

; This is where program-set variables are stored,
; i.e. where woody will store addresses, offsets, payloads, etc.
%include "elf/common/entry/variables.inc.s"
