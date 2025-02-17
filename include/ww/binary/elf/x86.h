/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x86.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:09:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 21:35:16 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef X86_H
# define X86_H
# undef X86_H
# ifndef __WW_ELF_X86_H__
#  define __WW_ELF_X86_H__

#  include <ww/elf/shared.h>

bool
ww_elf_write_x86(t_ww_args *args, int fd, t_ww_elf *elf, size_t total_size);

bool
ww_elf_write_x86_headers(t_ww_args *args, int fd, Elf32_Ehdr *ehdr,
	size_t total_size);

bool
ww_elf_write_x86_program(t_ww_args *args, int fd, Elf32_Ehdr *ehdr,
	size_t total_size);

# endif // __WW_ELF_X86_H__
#endif // X86_H
