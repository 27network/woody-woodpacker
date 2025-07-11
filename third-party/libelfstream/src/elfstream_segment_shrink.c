/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_segment_shrink.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:57:20 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/12 00:00:53 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_BITNESS
# include <elfstream.h>

# define ELF_BITNESS 32
# include "elfstream_segment_shrink.c"
# define ELF_BITNESS 64
# include "elfstream_segment_shrink.c"

void	elfstream_segment_shrink(
	t_elfstream *stream,
	t_elf_segment *segment
) {
	if (stream->bitness == ELFSTREAM_32)
		elfstream_segment_shrink_x32(stream, segment);
	else
		elfstream_segment_shrink_x64(stream, segment);
}

#else
# include <elfstream_macros.h>

FASTCALL void Func(update_segment)(t_elf_segment *segment, size_t position)
{
	(void) segment;
	(void) position;
}

FASTCALL void Func(update_section)(t_elf_section *section, size_t position)
{
	(void) section;
	(void) position;
}

FASTCALL void Func(elfstream_segment_shrink)(t_elfstream *stream, t_elf_segment *segment)
{
	Elf(Ehdr)	*ehdr = (Elf(Ehdr) *) &stream->ehdr32;
	Elf(Phdr)	*phdr = (Elf(Phdr) *) &segment->phdr32;
	size_t		segment_content_size = elfstream_content_size(segment->content);
	size_t		position = phdr->p_offset;

	size_t		i;
	for (i = 0; i < stream->section_count; i++) {
		Func(update_segment)(stream->segments + i, position);
	}
	for (i = 0; i < stream->section_count; i++) {
		Func(update_section)(stream->sections + i, position);
	}

	ehdr->e_shoff -= segment_content_size;
	elfstream_content_free(segment->content);
	segment->content = NULL;
	phdr->p_filesz -= 0;
}

# undef ELF_BITNESS
#endif // ELF_BITNESS
