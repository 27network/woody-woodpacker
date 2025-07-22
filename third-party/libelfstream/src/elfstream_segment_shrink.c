/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_segment_shrink.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:57:20 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/22 14:45:13 by kiroussa         ###   ########.fr       */
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

FASTCALL size_t Func(update_segment)(
	[[maybe_unused]] size_t seg_id,
	t_elf_segment *segment,
	size_t position,
	size_t segment_content_size
) {
	Elf(Phdr)	*phdr = (Elf(Phdr) *) &segment->phdr32;

	if (phdr->p_offset > position)
	{
		DBG("offsetting segment %zu of %zu (pos %#lx, filesz %#lx, memsz %#lx)", seg_id,
			(size_t) phdr->p_offset, (size_t) phdr->p_filesz, (size_t) phdr->p_memsz,
			segment_content_size);
		if (phdr->p_offset < segment_content_size)
			phdr->p_offset = segment_content_size;
		phdr->p_offset -= segment_content_size;
	}
	else if (phdr->p_offset + phdr->p_filesz > position)
	{
		Elf(Off) diff = position - phdr->p_offset + phdr->p_filesz;
		DBG("shrinking segment %zu of %zu (pos %#lx, filesz %#lx, memsz %#lx)", seg_id,
			(size_t) phdr->p_offset, (size_t) phdr->p_filesz, (size_t) phdr->p_memsz,
			diff);
		if (phdr->p_filesz < diff)
			phdr->p_filesz = diff;
		phdr->p_filesz -= diff;
	}
	return (segment_content_size);
}

FASTCALL size_t Func(update_section)(
	[[maybe_unused]] size_t sec_id,
	t_elf_section *section,
	size_t position,
	size_t segment_content_size
) {
	Elf(Shdr)	*shdr = (Elf(Shdr) *) &section->shdr32;

	if (shdr->sh_offset > position)
	{
		if (shdr->sh_offset < segment_content_size)
			shdr->sh_offset = segment_content_size;
		shdr->sh_offset -= segment_content_size;
	}
	else if (shdr->sh_offset + shdr->sh_size > position)
	{
		Elf(Off) diff = position - shdr->sh_offset + shdr->sh_size;
		if (shdr->sh_size < diff)
			shdr->sh_size = diff;
		shdr->sh_size -= diff;
	}
	return (segment_content_size);
}

FASTCALL void Func(elfstream_segment_shrink)(t_elfstream *stream, t_elf_segment *segment)
{
	Elf(Ehdr)	*ehdr = (Elf(Ehdr) *) &stream->ehdr32;
	Elf(Phdr)	*phdr = (Elf(Phdr) *) &segment->phdr32;
	size_t		segment_content_size = elfstream_content_size(segment->content);
	size_t		position = phdr->p_offset;
	size_t		seg_id = 0;

	while (seg_id < stream->segment_count)
	{
		if (stream->segments + seg_id == segment)
			break ;
		seg_id++;
	}

	DBG("shrinking segment %zu (pos %#lx, filesz %#lx, memsz %#lx)", seg_id,
		(size_t) position, (size_t) phdr->p_filesz, (size_t) phdr->p_memsz);

	// check alignment
	if (segment_content_size % 64 != 0)
		segment_content_size -= segment_content_size % 64;

	// realign based on the next segment
	// t_elf_segment	*closest = NULL;
	// size_t			closest_off = (size_t) -1;
	// size_t i;
	// for (i = 0; i < stream->segment_count; i++) {
	// 	Elf(Phdr) *tmp = (Elf(Phdr) *) &stream->segments[i].phdr32;
	// 	if (tmp->p_offset <= position)
	// 		continue ;
	// 	if (tmp->p_offset < closest_off)
	// 	{
	// 		DBG("found closer segment %zu (pos %#lx, filesz %#lx, memsz %#lx)", i,
	// 			(size_t) tmp->p_offset, (size_t) tmp->p_filesz, (size_t) tmp->p_memsz);
	// 		closest_off = tmp->p_offset;
	// 		closest = stream->segments + i;
	// 	}
	// }
	// DBG("segment_content_size: %#lx", (size_t) segment_content_size);
	// Elf(Phdr) *tmp2 = (Elf(Phdr) *) &closest->phdr32;
	// size_t diff = tmp2->p_offset - position;
	// DBG("diff: %#lx", (size_t) diff);
	// segment_content_size = diff;

	size_t offset = segment_content_size;
	size_t i;
	for (i = 0; i < stream->segment_count; i++) {
		DBG("segment %zu: offset %#lx, fzsize %#lx, memsz %#lx", i, (size_t) stream->segments[i].phdr64.p_offset,
			(size_t) stream->segments[i].phdr64.p_filesz, (size_t) stream->segments[i].phdr64.p_memsz);
		offset = Func(update_segment)(i, stream->segments + i, position, offset);
	}
	for (i = 0; i < stream->section_count; i++) {
		DBG("section %zu: offset %#lx, size %#lx", i, (size_t) stream->sections[i].shdr64.sh_offset,
			(size_t) stream->sections[i].shdr64.sh_size);
		Func(update_section)(i, stream->sections + i, position, segment_content_size);
	}
	ehdr->e_shoff -= segment_content_size;

	elfstream_content_free(segment->content);
	segment->content = NULL;
	phdr->p_filesz = 0;
}

# undef ELF_BITNESS
#endif // ELF_BITNESS
