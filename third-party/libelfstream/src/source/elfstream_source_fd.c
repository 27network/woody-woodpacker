/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_source_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:51:40 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/18 04:38:02 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <ft/mem.h>

t_content_source	*elfstream_source_fd(t_elfstream *stream, int fd,
						size_t offset, size_t size)
{
	t_content_source	*self;

	(void)stream;
	self = ft_calloc(1, sizeof(t_content_source));
	if (!self)
		return (NULL);
	self->type = CONTENT_SOURCE_FILE;
	self->write_fn = elfstream_write_source_fd;
	self->s_file.fd = fd;
	self->s_file.offset = offset;
	self->size = size;
	DBG("created elfstream_source_fd with fd %d, offset %#lx, size %zu (%#lx)",
		fd, offset, size, size);
	DBG("so like from %#lx to %#lx", offset, offset + size);
	return (self);
}
