/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_target.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:57:26 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 11:20:00 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

#ifndef ELF_BITNESS
# define ELF_BITNESS 32
# include "ww_bin_elf_target.c"
# define ELF_BITNESS 64
# include "ww_bin_elf_target.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

/**
 * This function finds the last (in virtual memory) PT_LOAD segment.
 */
t_elf_segment	*Func(ww_bin_elf_target)(t_elfstream *stream)
{
	t_elf_segment		*target;
	t_elf_segment		*tmp;
	int					i;
	Elf(Phdr)			*phdr;
	Elf(Phdr)			*tmp_phdr;

	i = 0;
	target = NULL;

	phdr = NULL;
	while (i < (int)stream->segment_count)
	{
		tmp = stream->segments + i++;
		tmp_phdr = (Elf(Phdr) *) &tmp->phdr32;
		if (tmp_phdr->p_type == PT_LOAD)
		{
			if (phdr && tmp_phdr->p_vaddr < phdr->p_vaddr)
				continue;
			target = tmp;
			phdr = tmp_phdr;
		}
	}
	return (target);
}

# undef ELF_BITNESS
#endif
