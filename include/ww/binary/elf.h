/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:24:16 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/15 03:03:47 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_H
# define ELF_H
# undef ELF_H
# ifndef __WW_BINARY_ELF_H__
#  define __WW_BINARY_ELF_H__

#  include <elf.h>
#  include <elfstream.h>
#  include <stdbool.h>
#  include <ww/binary.h>

typedef struct s_ww_elf_handler
{
	t_ww_binary_handler		base;
	t_elfstream				stream;
}	t_ww_elf_handler;

// #  define WW_SUPPORT_32 1

// typedef struct s_ww_elf
// {
// 	int						fd;
// 	size_t					size;
// 	union
// 	{
// 		Elf32_Ehdr	*e32;
// 		Elf64_Ehdr	*e64;
// 	};
// }	t_ww_elf;

// t_ww_elf
// *ww_elf_parse(const char *path);
//
// bool
// ww_elf_write(t_ww_args *args, t_ww_elf *elf, const char *path);
//
// void
// ww_elf_free(t_ww_elf **elf);

# endif // __WW_BINARY_ELF_H__
#endif // ELF_H
