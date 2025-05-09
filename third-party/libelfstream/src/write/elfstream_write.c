/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 07:38:33 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/09 14:33:54 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <sys/types.h>
#define ELF_BITNESS 32
#include "elfstream_write.template"
#define ELF_BITNESS 64
#include "elfstream_write.template"

enum e_elfstream_error	elfstream_write(t_elfstream *self, int fd)
{
	enum e_elfstream_error	error;

	if (self->bitness == ELFSTREAM_32)
	{
		error = elfstream_write_header_x32(self, fd);
		if (!error)
			error = elfstream_write_segments_x32(self, fd);
		if (!error)
			error = elfstream_write_sections_x32(self, fd);
	}
	else
	{
		error = elfstream_write_header_x64(self, fd);
		if (!error)
			error = elfstream_write_segments_x64(self, fd);
		if (!error)
			error = elfstream_write_sections_x64(self, fd);
	}
	return (error);
}
