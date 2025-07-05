BITS 32

%include "includes/defines.s"

global	subBytes
global	subBytes_32bits_words

subBytes:
	push	esi
	push	ebx
	push	ecx

	xor		ecx, ecx
	call	subBytes_loop

	pop		ecx
	pop		ebx
	pop		esi
	ret

subBytes_loop:
	mov		eax, [edi + 4 * ecx]
	call	subBytes_32bits_words
	bswap	eax
	mov		[edi + 4 * ecx], eax

	inc		ecx
	cmp		ecx, Nb
	jl		subBytes_loop

	ret

subBytes_32bits_words:
	push	esi
	push	ebx
	push	ecx

	xor		ecx, ecx
	xor		esi, esi

	mov		esi, eax
	xor		eax, eax
	jmp		loop

loop:
	xor		ebx, ebx
	mov		bx, si
	and		bx, 0x00FF
	call	getSboxEntry ;update eax with substitue words
	shr		esi, 8

	inc		ecx
	cmp		ecx, Nb
	jl		loop

	pop		ecx
	pop		ebx
	pop		esi
	ret

getSboxEntry:
	mov		al, [edx + ebx]
	cmp		ecx, Nb - 1
	jne		shiftLeft
	ret

shiftLeft:
	shl 	eax, 8
	ret
