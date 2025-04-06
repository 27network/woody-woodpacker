/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_read.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:28:08 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 18:02:47 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

t_ww_error	ww_bin_elf_read(t_ww_elf_handler *self, t_ww_binary *bin)
{
	enum e_elfstream_error	error;

	error = elfstream_open(&self->stream, bin->input_fd);
	if (error != ELFSTREAM_OK)
		return (ww_err_fmt(ERROR_IO, "failed to open elfstream: 0x%x\n",
				error));
	return (ww_ok());
}
