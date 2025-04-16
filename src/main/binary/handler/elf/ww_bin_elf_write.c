/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:39:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/16 17:23:35 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

t_ww_error	ww_bin_elf_write(t_ww_elf_handler *self, t_ww_binary *bin)
{
	enum e_elfstream_error	err;
	int						fd;

	fd = open(bin->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (ww_err_fmt(ERROR_IO, "failed to open output file '%s': %m\n",
				bin->output));
	err = elfstream_write(&self->stream, );
	(void)elfstream_close(&self->stream);
	if (err != ELFSTREAM_OK)
		return (ww_err_fmt(ERROR_IO, "failed to write elfstream: 0x%x\n",
				err));
	return (ww_ok());
}
