/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_source_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:51:40 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/04 01:44:29 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <ft/mem.h>

enum e_elfstream_error
elfstream_write_source_fd(t_content_source *self, int fd);

t_content_source	*elfstream_source_fd(int fd, size_t offset, size_t size)
{
	t_content_source	*self;

	self = ft_calloc(1, sizeof(t_content_source));
	if (!self)
		return (NULL);
	self->type = CONTENT_SOURCE_FILE;
	self->write_fn = elfstream_write_source_fd;
	self->s_file.fd = fd;
	self->s_file.offset = offset;
	self->s_file.size = size;
	return (self);
}
