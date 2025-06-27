%include "elf/common/decrypt/aes_32/includes/defines.inc.s"
%include "elf/common/decrypt/aes_32/addRoundKey.inc.s"
%include "elf/common/decrypt/aes_32/decryption/invMixColumns.inc.s"
%include "elf/common/decrypt/aes_32/decryption/invRoundTransformations.inc.s"
%include "elf/common/decrypt/aes_32/decryption/invShiftRows.inc.s"
%include "elf/common/decrypt/aes_32/decryption/invSubBytes.inc.s"
%include "elf/common/decrypt/aes_32/includes/variables.inc.s"

decrypt:
	call	inv_first_round_transformation
	call	inv_middle_round_transformations
	call	inv_last_round_transformations

	ret
