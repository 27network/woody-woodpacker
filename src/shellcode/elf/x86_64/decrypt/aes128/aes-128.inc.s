%include "elf/x86_64/decrypt/aes128/includes/defines.inc.s"
%include "elf/x86_64/decrypt/aes128/addRoundKey.inc.s"
%include "elf/x86_64/decrypt/aes128/initialize_registers.inc.s"
%include "elf/x86_64/decrypt/aes128/decryption/invMixColumns.inc.s"
%include "elf/x86_64/decrypt/aes128/decryption/invRoundTransformations.inc.s"
%include "elf/x86_64/decrypt/aes128/decryption/invShiftRows.inc.s"
%include "elf/x86_64/decrypt/aes128/decryption/invSubBytes.inc.s"
%include "elf/x86_64/decrypt/aes128/includes/variables.inc.s"

decrypt:
	call	initialize_registers

	call	inv_first_round_transformation
	call	inv_middle_round_transformations
	call	inv_last_round_transformations

	movdqu	[rdi], xmm0

	ret
