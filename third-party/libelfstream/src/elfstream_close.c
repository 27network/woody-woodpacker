/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:10:55 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/17 10:40:04 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

static inline void	elfstream_segment_free(t_elf_segment *segment)
{
	t_content_source	*tmp;

	while (segment->content)
	{
		tmp = segment->content;
		segment->content = segment->content->next;
		free(tmp);
	}
}

enum e_elfstream_error	elfstream_close(t_elfstream *self)
{
	size_t				i;

	if (!self)
		return (ELFSTREAM_ERROR);
	if (self->segments)
	{
		i = 0;
		while (i++ < self->segment_count)
			elfstream_segment_free(self->segments + i - 1);
		free(self->segments);
	}
	if (self->sections)
		free(self->sections);
	if (self->fd > 2)
		close(self->fd);
	self->fd = 0;
	self->segments = NULL;
	self->sections = NULL;
	return (ELFSTREAM_OK);
}
