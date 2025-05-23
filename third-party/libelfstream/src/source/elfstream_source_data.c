/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_source_data.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 17:03:05 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/12 20:28:14 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <ft/mem.h>

t_content_source	*elfstream_source_data(const char *data, size_t size,
						bool allocated)
{
	t_content_source	*self;

	self = ft_calloc(1, sizeof(t_content_source));
	if (!self)
		return (NULL);
	self->type = CONTENT_SOURCE_MEMORY;
	self->write_fn = elfstream_write_source_data;
	self->s_memory.data = data;
	self->s_memory.allocated = allocated;
	self->size = size;
	return (self);
}
