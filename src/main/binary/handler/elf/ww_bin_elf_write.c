/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:39:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/12 20:19:41 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ww/binary/elf.h>

t_ww_error	ww_bin_elf_write(t_ww_elf_handler *self, t_ww_binary *bin)
{
	enum e_elfstream_error	err;
	int						fd;

	ww_debug("ww_bin_elf_write(handler=%p)\n", (void *)self);
	fd = open(bin->output, O_WRONLY | O_CREAT, 0755);
	if (fd == -1)
		return (ww_err_fmt(ERROR_IO, "failed to open output file '%s': %m\n",
				bin->output));
	ww_trace("Opened output file '%s'\n", bin->output);
	ww_trace("Writing ELFstream at %p\n", (void *)&self->stream);
	ww_trace("ELFstream segments: %p\n", (void *)self->stream.segments);
	err = elfstream_write(&self->stream, fd);
	close(fd);
	if (err != ELFSTREAM_OK)
		return (ww_err_fmt(ERROR_IO, "failed to write elfstream: 0x%x\n",
				err));
	return (ww_ok());
}
