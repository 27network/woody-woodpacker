/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:50:24 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/22 01:21:20 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_BITNESS
# include <ft/mem.h>
# include <ww/binary/elf.h>

# define ELF_BITNESS 32
# include "ww_bin_elf_entry.c"
# define ELF_BITNESS 64
# include "ww_bin_elf_entry.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

Elf(Off)	Func(ww_bin_elf_entry)(
	[[maybe_unused]] t_ww_elf_handler *self,
	t_elf_segment *orig,
	Elf(Off) offset
) {
	Elf(Phdr)	*phdr;

	phdr = (Elf(Phdr) *) &orig->phdr32;
	phdr->p_flags |= PF_R | PF_W | PF_X;
	ww_trace("Providing woody entrypoint at vaddr %#lx + offset %#lx\n", (size_t)phdr->p_vaddr, (size_t)offset);
	return (phdr->p_vaddr + offset);
}

# undef ELF_BITNESS
#endif
