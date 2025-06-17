; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    variables.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 21:06:17 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/17 15:41:45 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Offset from _woody_entry to _start, used when jumping to the original ELF entry
start_offset: DECLARE_SIZE 0x41414141

; Offset from _woody_entry to the decryption routine
decryption_routine_offset: DECLARE_SIZE 0x42424242

; Offset from _woody_entry to the decompression routine
decompression_routine_offset: DECLARE_SIZE 0x43434343


; The decryption key, if applicable
encryption_key: times 0x10 db 0x42


; Whether the loader should waitpid/wait4 for the forked payload.
; This is only applicable when loading an ELF/executable file.
loader_async: db 0

_padding2: times 0x7 db 0x69 ; Padding

; Size of the `user_payload` buffer
user_payload_size: DECLARE_SIZE 0x44444444


; Offset from _woody_entry to the write offset of the ELF segments
segments_write_offset: DECLARE_SIZE 0x45454545

; Size of the `segments_content` buffer
segments_content_size: DECLARE_SIZE 0x46464646


; The content of the ELF segments that we'll decrypt and decompress
; (note: filled at runtime by woody)
segments_content: DECLARE_SIZE 0x47474747

; The payload itself loaded by _woody_loader, user defined
; (note: filled at runtime by woody)
user_payload: DECLARE_SIZE 0x48484848
