; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    variables.inc.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 21:06:17 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/15 15:15:09 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

start_offset: DECLARE_SIZE 0x42424242
payload_size: DECLARE_SIZE 0x42424242
loader_fork: db 1
payload: db 0xc3
tmp_data: times 0x1000 db 0
