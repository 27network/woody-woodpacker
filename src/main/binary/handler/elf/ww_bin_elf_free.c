/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 09:23:18 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/17 10:31:57 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

t_ww_error	ww_bin_elf_free(t_ww_elf_handler *self, t_ww_binary *bin)
{
	ww_debug("ww_bin_elf_free(handler=%p)\n", (void *)self);
	ww_trace("Closing ELFstream at %p\n", (void *)&self->stream);
	ww_trace("ELFstream segments: %p\n", (void *)self->stream.segments);
	(void)elfstream_close(&self->stream);
	(void)bin;
	return (ww_ok());
}
