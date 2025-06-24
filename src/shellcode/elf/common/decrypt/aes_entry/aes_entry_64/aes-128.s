%include "addRoundKey.s"			;remove encryption related stuff
%include "initialize_registers.s"	;idem
%include "decryption/invMixColumns.s"
%include "decryption/invRoundTransformations.s"
%include "decryption/invShiftRows.s"
%include "decryption/invSubBytes.s"
%include "includes/sections.s"
%include "includes/defines.s"

decrypt:
	lea		rdx, [rel inv_mixcolumns_matrix]

	call	inv_first_round_transformation
	call	inv_middle_round_transformations
	call	inv_last_round_transformations

	movdqa	[rdi], xmm0

	ret
