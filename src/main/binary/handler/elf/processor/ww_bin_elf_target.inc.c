/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_target.inc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:57:26 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/21 17:54:27 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_elf_segment	*Func(ww_bin_elf_find_target)(t_elfstream *stream)
{
	t_elf_segment		*target;
	t_elf_segment		*tmp;
	int					i;
	Elf(Phdr)			*phdr;
	Elf(Phdr)			*tmp_phdr;

	i = -1;
	target = NULL;

	phdr = NULL;
	while (i++ < (int)stream->segment_count)
	{
		tmp = stream->segments + i;
		tmp_phdr = (Elf(Phdr) *) &tmp->phdr32;
		if ((tmp_phdr->p_type & PT_LOAD) == PT_LOAD)
		{
			if (phdr && tmp_phdr->p_vaddr < phdr->p_vaddr)
				continue;
			target = tmp;
			phdr = tmp_phdr;
		}
	}
	return (target);
}
