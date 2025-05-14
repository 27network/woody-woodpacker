; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    entrypoint.inc.s                                   :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/03/30 15:34:44 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/15 00:32:36 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel
global _woody_entry 

_woody_entry:
	; The plan is simple:

	; 1. call _woody_loader to execute the provided payload
	; call [rel _woody_loader]

	; 2. call _woody_decrypt

	; 3. call _woody_decompress
	
	; 4. jump to _start
	jmp [rel start_addr] ; yeet!

%include "elf/common/loader.inc.s"
; %include "decrypt.inc.s"
; %include "decompress.inc.s"

; This is where program-set variables are stored,
; i.e. where woody will store addresses, offsets, payloads, etc.
%include "elf/common/variables.inc.s"
