; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    syscalls.inc.s                                     :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2025/05/14 20:14:01 by kiroussa          #+#    #+#              ;
;    Updated: 2025/05/14 23:47:18 by kiroussa         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%define SYS_WRITE 1
%define SYS_CLOSE 57

%define SYS_EXIT 60

%define SYS_MEMFD_CREATE 319

%define SYS_FORK 57
%define SYS_EXECVEAT 281
%define SYS_WAIT4 61
