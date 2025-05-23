/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:10:55 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/12 17:14:29 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

enum e_elfstream_error	elfstream_close(t_elfstream *self)
{
	size_t				i;

	if (!self)
		return (ELFSTREAM_ERROR);
	if (self->segments)
	{
		i = 0;
		while (i++ < self->segment_count)
			elfstream_content_free(self->segments[i - 1].content);
		free(self->segments);
	}
	if (self->sections)
	{
		i = 0;
		while (i++ < self->section_count)
			elfstream_content_free(self->sections[i - 1].content);
		free(self->sections);
	}
	self->fd = 0;
	self->segments = NULL;
	self->sections = NULL;
	return (ELFSTREAM_OK);
}
