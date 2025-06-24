; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    loader.inc.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:11:06 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/24 17:26:08 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Variables (see variables.inc.s):
;   - payload
;   - payload_size

_woody_loader:
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

	push RAX
	call do_execve
	pop  RAX
	jmp  .Lend

.Lcheck_interpret:
 	cmp  BYTE [RDX], 0x23	 ; #
 	jne  .Lshellcode
 	cmp  BYTE [RDX + 1], 0x21 ; !
 	jne  .Lshellcode
 	cmp  BYTE [RDX + 2], 0x2f ; /
 	jne  .Lshellcode

	push RAX
	call do_execve
	pop  RAX
	jmp  .Lend

.Lshellcode:
	push RAX
  	call RDX
	pop  RAX

.Lend:
	ret


do_execve:
	call create_payload_fd

memfd_name: db "le_wood", 0

create_payload_fd:
	pop SYS_ARG0
	mov SYS_ARG1, 0
	mov SYS_NUM, SYS_MEMFD_CREATE
	SYSCALL

	cmp	 RAX, -1
	je   .Lend

.Lwrite_payload_to_fd:
	; Save fd to sys_arg4
	mov  SYS_ARG4, RAX

	mov  SYS_ARG0, RAX
	mov  SYS_ARG1, RDX

	pop  SYS_ARG2
	pop  RAX
	push RAX
	push SYS_ARG2

	mov  SYS_ARG2, [RAX + OFFSET_USER_PAYLOAD_SIZE] 

	mov  SYS_NUM, SYS_WRITE
	SYSCALL

	cmp	 RAX, -1
	je   .Lend

.Lexecute_payload:
	mov SYS_NUM, SYS_FORK
	SYSCALL

	cmp RAX, -1
	je  .Lend
	cmp RAX, 0
	je  .Lexec

	mov SYS_ARG0, RAX ; restore fd (from write(2))

	pop  SYS_ARG2
	pop  RAX
	push RAX
	push SYS_ARG2

	cmp BYTE [RAX + OFFSET_LOADER_ASYNC], 1
	je  .Lend

	xor SYS_ARG1, SYS_ARG1
	xor SYS_ARG2, SYS_ARG2
	xor SYS_ARG3, SYS_ARG3
	mov SYS_NUM, SYS_WAIT4
	SYSCALL

	jmp .Lend

.Lexec:
%define AT_EMPTY_PATH 0x1000
	mov  SYS_ARG0, SYS_ARG4			; restore dirfd
	push 0
	mov  SYS_ARG1, RSP				; pathname
	mov	 SYS_ARG2, 0				; argv
	mov  SYS_ARG3, RSP
	add  SYS_ARG3, 0x38				; envp
	mov  SYS_ARG4, AT_EMPTY_PATH	; flags
	mov  SYS_NUM, SYS_EXECVEAT
	SYSCALL
	pop  SYS_ARG4 ; little cleanup

	mov  SYS_ARG0, 0
	mov  SYS_NUM, SYS_EXIT
	SYSCALL

	ret

.Lend:
	ret
