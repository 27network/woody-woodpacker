; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    variables.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 21:06:17 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/21 20:01:29 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Offset from _woody_entry to _start, used when jumping to the original ELF entry
start_offset: DECLARE_SIZE 0x42424242

; Whether the loader should waitpid/wait4 for the forked payload
loader_async: db 0

; Size of the `payload` buffer
payload_size: DECLARE_SIZE 0x42424242

; Whether the loader should waitpid/wait4 for the forked payload
encryption_key: times 0x64 db 0x42

; Size of the `segments_content` buffer
segments_content_size: DECLARE_SIZE 0x42424242

; Offset from _woody_entry to the write offset of the ELF segments
segments_write_offset: DECLARE_SIZE 0x42424242

; The content of the ELF segments that we'll decrypt and decompress
; (note: filled at runtime by woody)
segments_content: DECLARE_SIZE 0x42424242

; The payload itself
; (note: filled at runtime by woody)
payload: DECLARE_SIZE 0x42424242
