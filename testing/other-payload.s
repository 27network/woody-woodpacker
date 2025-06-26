bits 64
default rel

_payload:
	call _payload_entry

message: db "This is an injected bit of code :3", 10
message_len: equ $-message

_payload_entry:
	mov rdi, 1 ; stdout
	pop rsi ; message
	mov rdx, message_len ; message_len
	mov rax, 1 ; SYS_WRITE
	syscall

	ret
