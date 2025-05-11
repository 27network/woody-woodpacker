/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_source_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 16:51:40 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/11 10:14:32 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <ft/mem.h>

// static inline size_t	pick(t_elfstream *self, size_t off_32, size_t off_64)
// {
// 	if (self->bitness == ELFSTREAM_32)
// 		return (off_32);
// 	return (off_64);
// }
//
// static inline size_t	elfstream_current_offset(t_elfstream *stream,
// 							size_t self_offset)
// {
// 	size_t	current_offset;
// 	size_t	i;
//
// 	current_offset = pick(stream, sizeof(Elf32_Ehdr), sizeof(Elf64_Ehdr));
// 	current_offset += stream->segment_count * pick(stream, sizeof(Elf32_Phdr),
// 			sizeof(Elf64_Phdr));
// 	(void)self_offset;
// 	i = 0;
// 	while (i < stream->segment_count && stream->segments[i].content)
// 	{
// 		// TODO: account for all segment sizes, but this is usually fine
// 		// since this is on first read, so only one content source should
// 		// be active at a time
// 		current_offset += elfstream_content_size(stream->segments[i].content);
// 		i++;
// 	}
// 	return (current_offset);
// }

t_content_source	*elfstream_source_fd(t_elfstream *stream, int fd,
						size_t offset, size_t size)
{
	t_content_source	*self;
	// size_t				current_stream_offset;
	// size_t				correction;

	(void)stream;
	self = ft_calloc(1, sizeof(t_content_source));
	if (!self)
		return (NULL);
	self->type = CONTENT_SOURCE_FILE;
	self->write_fn = elfstream_write_source_fd;
	// current_stream_offset = elfstream_current_offset(stream, offset);
	// if (offset < current_stream_offset)
	// {
	// 	DBG("offset is before current stream offset, truncating to after that");
	// 	correction = current_stream_offset - offset;
	// 	offset += correction;
	// 	if (correction >= size)
	// 		size = 0;
	// 	else
	// 		size -= correction;
	// 	DBG("new offset=%#lx size=%#lx", offset, size);
	// }
	self->s_file.fd = fd;
	self->s_file.offset = offset;
	self->size = size;
	DBG("created elfstream_source_fd with fd %d, offset %#lx, size %zu (%#lx)",
		fd, offset, size, size);
	DBG("so like from %#lx to %#lx", offset, offset + size);
	return (self);
}
