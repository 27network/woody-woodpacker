/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:39:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 18:04:10 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

static inline void	ww_bin_elf_cleanup(t_ww_elf_handler *self)
{
	(void)elfstream_close(&self->stream);
}

t_ww_error	ww_bin_elf_write(t_ww_elf_handler *self, t_ww_binary *bin)
{
	(void) bin;
	ww_bin_elf_cleanup(self);
	return (ww_ok());
}
