; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    aes.entry.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/26 21:32:28 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/26 16:40:26 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

bits BITS
default rel


; 64 BITS ARCHITECTURE
%if BITS == 64
_woody_decrypt_aes:
	call _woody_decrypt_aes_x64

	ret

%include "elf/common/decrypt/aes_64/aes.entry_64.s"


; 32 BITS ARCHITECTURE
%else
_woody_decrypt_aes:
	call _woody_decrypt_aes_x86

	ret

%include "elf/common/decrypt/aes_32/aes.entry_32.s"

%endif
