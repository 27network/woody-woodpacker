/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_source_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:51:40 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/13 16:53:33 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <ft/mem.h>

t_content_source	*elfstream_source_fd(int fd, size_t offset, size_t size)
{
	t_content_source	*self;

	self = ft_calloc(1, sizeof(t_content_source));
	if (!self)
		return (NULL);
	self->type = CONTENT_SOURCE_FILE;
	self->s_file.fd = fd;
	self->s_file.offset = offset;
	self->s_file.size = size;
	return (self);
}
