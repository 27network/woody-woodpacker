%include "encrypt/aes128/includes/defines.inc.s"
%include "encrypt/aes128/includes/variables.inc.s"
%include "encrypt/aes128/addRoundKey.inc.s"
%include "encrypt/aes128/initialize_registers.inc.s"
%include "encrypt/aes128/encryption/keyExpansion.inc.s"
%include "encrypt/aes128/encryption/mixColumns.inc.s"
%include "encrypt/aes128/encryption/roundTransformations.inc.s"
%include "encrypt/aes128/encryption/shiftRows.inc.s"
%include "encrypt/aes128/encryption/subBytes.inc.s"

%macro xpush 1
    ; Push XMM register to stack (unaligned)
    sub rsp, 16
    movups [rsp], %1
%endmacro

%macro xpop 1
    ; Pop XMM register from stack (unaligned)
    movups %1, [rsp]
    add rsp, 16
%endmacro

global aes128_encrypt
aes128_encrypt:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
	xpush xmm0
	xpush xmm1
	xpush xmm2
	xpush xmm3
	xpush xmm4
	xpush xmm5
	xpush xmm6
	xpush xmm7
	xpush xmm8
	xpush xmm9
	xpush xmm10
	xpush xmm11
	xpush xmm12
	xpush xmm13
	xpush xmm14
	xpush xmm15
	push rsp

	call	initialize_registers
	call	keyExpansion

	call	first_round_transformation
	call	middle_round_transformations
	call	last_round_transformations

	movdqa	[rdi], xmm0

	pop rsp
	xpop xmm15
	xpop xmm14
	xpop xmm13
	xpop xmm12
	xpop xmm11
	xpop xmm10
	xpop xmm9
	xpop xmm8
	xpop xmm7
	xpop xmm6
	xpop xmm5
	xpop xmm4
	xpop xmm3
	xpop xmm2
	xpop xmm1
	xpop xmm0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	ret
