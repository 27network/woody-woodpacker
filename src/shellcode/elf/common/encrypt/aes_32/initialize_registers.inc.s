initialize_registers:
	mov	edi, [esp + 0x08]
	lea	esi, [rel key]
	lea	edx, [rel s_box]
	lea	ebx, [rel mixcolumns_matrix]
	xor	eax, eax
	xor	ecx, ecx

	ret
