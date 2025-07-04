; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/07/04 17:40:31 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel

_woody_decrypt_aes:
	jmp _woody_decrypt_aes_entry

%if BITS == 64
%include "elf/common/decrypt/aes_64/aes.entry_64.inc.s"
%else
%include "elf/common/decrypt/aes_32/aes.entry_32.inc.s"
%endif
