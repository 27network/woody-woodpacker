%include "elf/common/decrypt/aes_64/includes/defines.inc.s"
%include "elf/common/decrypt/aes_64/addRoundKey.inc.s"
%include "elf/common/decrypt/aes_64/initialize_registers.inc.s"
%include "elf/common/decrypt/aes_64/decryption/invMixColumns.inc.s"
%include "elf/common/decrypt/aes_64/decryption/invRoundTransformations.inc.s"
%include "elf/common/decrypt/aes_64/decryption/invShiftRows.inc.s"
%include "elf/common/decrypt/aes_64/decryption/invSubBytes.inc.s"
%include "elf/common/decrypt/aes_64/includes/variables.inc.s"

decrypt:
	call	initialize_registers

	call	inv_first_round_transformation
	call	inv_middle_round_transformations
	call	inv_last_round_transformations

	movdqa	[rdi], xmm0

	ret
