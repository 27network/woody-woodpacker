/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:39:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/15 03:05:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

t_ww_error	ww_bin_elf_write(t_ww_elf_handler *self, t_ww_binary *bin)
{
	const enum e_elfstream_error	err
		= elfstream_write(&self->stream, bin->output_fd);

	(void)elfstream_close(&self->stream);
	if (err != ELFSTREAM_OK)
		return (ww_err_fmt(ERROR_IO, "failed to write elfstream: 0x%x\n",
				err));
	return (ww_ok());
}
