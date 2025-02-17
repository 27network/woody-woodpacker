/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_elf_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:47:46 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 15:51:49 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/io.h>
#include <ft/mem.h>
#include <sys/mman.h>
#include <ww/elf.h>

void	ww_elf_free(t_ww_elf **elf_ptr)
{
	t_ww_elf	*elf;

	if (elf_ptr && *elf_ptr)
	{
		elf = *elf_ptr;
		ft_closep(&elf->fd);
		if (elf->e32)
			munmap((void *) elf->e32, elf->size);
	}
	ft_freep((void **) elf_ptr);
}
