/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_identifier.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:51:46 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/02 17:12:27 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/elf.h>
#include <ww/log.h>

t_ww_error	ww_bin_elf_identifier(t_ww_binary *bin, bool *identified)
{
	Elf32_Ehdr		tmp;

	ft_memset(&tmp, 0, sizeof(Elf32_Ehdr));
	if ((size_t)read(bin->input_fd, &tmp, EI_NIDENT) != EI_NIDENT)
		return (ww_err_str(ERROR_IO, "Invalid ELF header (read failed)"));
	if (ft_memcmp(&tmp.e_ident, ELFMAG, SELFMAG) != 0)
		return (ww_err_str(ERROR_IO, "Invalid ELF header (magic mismatch)"));
	*identified = true;
	if (tmp.e_ident[EI_CLASS] != ELFCLASS32
		&& tmp.e_ident[EI_CLASS] != ELFCLASS64)
		return (ww_err_fmt(ERROR_IO, "Invalid ELF class: %d\n",
				tmp.e_ident[EI_CLASS]));
	return (ww_ok());
}
