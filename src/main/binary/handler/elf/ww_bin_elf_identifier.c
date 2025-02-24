/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_identifier.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:51:46 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/24 17:45:51 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/elf.h>
#include <ww/log.h>

enum e_ww_error	ww_bin_elf_identifier(t_ww_binary *bin)
{
	Elf32_Ehdr		tmp;

	ft_memset(&tmp, 0, sizeof(Elf32_Ehdr));
	if ((size_t)read(bin->input_fd, &tmp, EI_NIDENT) != EI_NIDENT)
		return (WW_ERROR);
	if (ft_memcmp(&tmp.e_ident, ELFMAG, SELFMAG) != 0)
		return (WW_ERROR);
	if (tmp.e_ident[EI_CLASS] != ELFCLASS32
		&& tmp.e_ident[EI_CLASS] != ELFCLASS64)
	{
		ww_error("Invalid ELF class: %d\n", tmp.e_ident[EI_CLASS]);
		return (WW_ERROR_FORMAT);
	}
	return (WW_OK);
}
