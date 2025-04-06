/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_identifier.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:51:46 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:51:20 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/elf.h>
#include <ww/log.h>

static inline const char	*ww_elf_strtype(Elf32_Half type)
{
	static const char	*types[] = {
	[ET_NONE] = "NONE",
	[ET_REL] = "REL",
	[ET_EXEC] = "EXEC",
	[ET_DYN] = "DYN",
	[ET_CORE] = "CORE",
	};

	if (type < 0 || type > ET_CORE)
		return ("unknown");
	return (types[type]);
}

t_ww_error	ww_bin_elf_identifier(t_ww_binary *bin, bool *identified)
{
	const size_t	read_size = sizeof(Elf32_Ehdr);
	Elf32_Ehdr		tmp;

	ft_memset(&tmp, 0, sizeof(Elf32_Ehdr));
	if ((size_t)read(bin->input_fd, &tmp, read_size) != read_size)
		return (ww_err_str(ERROR_IO, "Invalid ELF header (read failed)"));
	if (ft_memcmp(&tmp.e_ident, ELFMAG, SELFMAG) != 0)
		return (ww_err_str(ERROR_IO, "Invalid ELF header (magic mismatch)"));
	*identified = true;
	if (tmp.e_ident[EI_CLASS] != ELFCLASS32
		&& tmp.e_ident[EI_CLASS] != ELFCLASS64)
		return (ww_err_fmt(ERROR_IO, "Invalid ELF class: %d\n",
				tmp.e_ident[EI_CLASS]));
	ww_trace("Identified %d bit ELF file\n", tmp.e_ident[EI_CLASS] * 32);
	if (tmp.e_type != ET_DYN && tmp.e_type != ET_EXEC)
		return (ww_err_fmt(ERROR_IO, "Invalid ELF type: %s (expected "
				"DYN or EXEC)\n", ww_elf_strtype(tmp.e_type)));
	return (ww_ok());
}
