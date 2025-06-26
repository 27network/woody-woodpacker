%include "elf/common/decrypt/aes/includes/defines.inc.s"
%include "elf/common/decrypt/aes/addRoundKey.inc.s"			;remove encryption related stuff
%include "elf/common/decrypt/aes/initialize_registers.inc.s"	;idem
%include "elf/common/decrypt/aes/decryption/invMixColumns.inc.s"
%include "elf/common/decrypt/aes/decryption/invRoundTransformations.inc.s"
%include "elf/common/decrypt/aes/decryption/invShiftRows.inc.s"
%include "elf/common/decrypt/aes/decryption/invSubBytes.inc.s"
%include "elf/common/decrypt/aes/includes/variables.inc.s"

decrypt:
	call	inv_first_round_transformation
	call	inv_middle_round_transformations
	call	inv_last_round_transformations

	movdqa	[rdi], xmm0

	ret
