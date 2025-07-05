%include "includes/defines.inc.s"
%include "includes/variables.inc.s"
%include "addRoundKey.inc.s"
%include "initialize_registers.inc.s"
%include "encryption/keyExpansion.inc.s"
%include "encryption/mixColumns.inc.s"
%include "encryption/roundTransformations.inc.s"
%include "encryption/shiftRows.inc.s"
%include "encryption/subBytes.inc.s"

encrypt:
	call	initialize_registers
	call	keyExpansion

	call	first_round_transformation
	call	middle_round_transformations
	call	last_round_transformations

	movdqa	[rdi], xmm0

	ret
