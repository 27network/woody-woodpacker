%include "encrypt/aes128/includes/defines.inc.s"
%include "encrypt/aes128/includes/variables.inc.s"
%include "encrypt/aes128/addRoundKey.inc.s"
%include "encrypt/aes128/initialize_registers.inc.s"
%include "encrypt/aes128/encryption/keyExpansion.inc.s"
%include "encrypt/aes128/encryption/mixColumns.inc.s"
%include "encrypt/aes128/encryption/roundTransformations.inc.s"
%include "encrypt/aes128/encryption/shiftRows.inc.s"
%include "encrypt/aes128/encryption/subBytes.inc.s"

aes128_encrypt:
	call	initialize_registers
	call	keyExpansion

	call	first_round_transformation
	call	middle_round_transformations
	call	last_round_transformations

	movdqa	[rdi], xmm0

	ret
