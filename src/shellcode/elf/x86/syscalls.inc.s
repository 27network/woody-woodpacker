; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    syscalls.inc.s                                     :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 23:38:03 by kiroussa          #+#    #+#              ;
;    Updated: 2025/06/24 17:24:19 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%define SYS_WRITE 4
%define SYS_CLOSE 6

%define SYS_EXIT 1

%define SYS_MEMFD_CREATE 356

%define SYS_FORK 2
%define SYS_EXECVEAT 358
%define SYS_WAIT4 114
