global _woody

_woody:
	push rbp
	mov rax, 0x1060
	call rax
	pop rbp
	ret
