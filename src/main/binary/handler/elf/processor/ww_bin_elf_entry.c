/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:50:24 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/19 17:00:49 by kiroussa         ###   ########.fr       */
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

FASTCALL void	Func(ww_bin_elf_entry_section_adjust)(t_elf_segment *segment,
														Elf(Off) offset)
{
	Elf(Shdr)	*shdr;
	Elf(Shdr)	*target;
	size_t		i;

	target = NULL;
	i = 0;
	while (i < segment->stream->section_count)
	{
		shdr = (Elf(Shdr) *) &segment->stream->sections[i].shdr32;
		if (shdr->sh_offset <= offset && shdr->sh_offset + shdr->sh_size > offset)
			target = shdr;
		i++;
	}
	if (target)
		target->sh_flags |= SHF_WRITE | SHF_EXECINSTR;
}

Elf(Off)	Func(ww_bin_elf_entry)(t_ww_elf_handler *self, t_elf_segment *orig,
							   Elf(Off) offset)
{
	Elf(Phdr)	*phdr;

	phdr = (Elf(Phdr) *) &orig->phdr32;
	phdr->p_flags |= PF_R | PF_W | PF_X;
	return (phdr->p_vaddr + offset);
}

# undef ELF_BITNESS
#endif
