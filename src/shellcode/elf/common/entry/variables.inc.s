; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    variables.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 21:06:17 by kiroussa          #+#    #+#              ;
;    Updated: 2025/08/07 20:08:40 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

OFFSET_WOODY_START_BASE equ (_woody_start_base - _woody_start)
OFFSET_START_OFFSET equ (start_offset - _woody_start)
OFFSET_DECRYPTION_ROUTINE_OFFSET equ (decryption_routine_offset - _woody_start)
OFFSET_DECOMPRESSION_ROUTINE_OFFSET equ (decompression_routine_offset - _woody_start)
OFFSET_ENCRYPTION_KEY equ (encryption_key - _woody_start)
OFFSET_LOADER_ASYNC equ (loader_async - _woody_start)
OFFSET_USER_PAYLOAD_SIZE equ (user_payload_size - _woody_start)
OFFSET_SEGMENTS_WRITE_OFFSET equ (segments_write_offset - _woody_start)
OFFSET_SEGMENTS_CONTENT_SIZE equ (segments_content_size - _woody_start)
OFFSET_SEGMENTS_CONTENT equ (segments_content - _woody_start)

; **NOTE**: The variable layout (order and size) in this file is important,
;           and is directly linked with `ww_bin_elf_payload_build`.

; Offset from _woody_entry to _start, used when jumping to the original ELF entry
start_offset: DECLARE_SIZE 0x41414141

; Offset from _woody_entry to the decryption routine
decryption_routine_offset: DECLARE_SIZE 0x42424242

; Offset from _woody_entry to the decompression routine
decompression_routine_offset: DECLARE_SIZE 0x43434343


; The decryption key, if applicable
encryption_key: times 0x10 db 0x42
_encryption_key_padding: times 240 db 0x00


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
