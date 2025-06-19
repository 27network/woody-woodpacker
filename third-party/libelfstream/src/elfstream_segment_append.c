/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_segment_append.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:30:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/18 00:45:38 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_BITNESS
# include <elfstream.h>

# define ELF_BITNESS 32
# include "elfstream_segment_append.c"
# define ELF_BITNESS 64
# include "elfstream_segment_append.c"

size_t	elfstream_segment_append(t_elfstream *stream, t_elf_segment *segment,
			t_content_source *content)
{
	t_content_source	*tmp;
	const size_t		size = elfstream_content_size(content);

	if (size > 0)
	{
		tmp = segment->content;
		if (!tmp)
			segment->content = content;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = content;
		}
	}
	if (stream->bitness == ELFSTREAM_32)
		return (update_stream_x32(stream, segment, size));
	return (update_stream_x64(stream, segment, size));
}

void elfstream_segment_offset(t_elfstream *stream, size_t size, size_t position)
{
	if (stream->bitness == ELFSTREAM_32)
		elfstream_segment_offset_x32(stream, size, position);
	else
		elfstream_segment_offset_x64(stream, size, position);
}

void elfstream_section_offset(t_elfstream *stream, size_t size, size_t position)
{
	if (stream->bitness == ELFSTREAM_32)
		elfstream_section_offset_x32(stream, size, position);
	else
		elfstream_section_offset_x64(stream, size, position);
}

#else
# include <elfstream_macros.h>

/**
 * Here we want to handle properly changing the offsets/sizes of the
 * ELF Segments relative to our injection content.
 */
FASTCALL void
Func(elfstream_segment_offset)(t_elfstream *stream, size_t size, size_t position)
{
	size_t		i;
	Elf(Phdr)	*phdr;

	i = 0;
	DBG("injecting code (%#lx) at %#lx", size, position);
	while (i < stream->segment_count)
	{
		phdr = (Elf(Phdr) *) &stream->segments[i].phdr32;
		DBG("checking offsetting phdr[%d]: %p", i, phdr);
		DBG("phdr[%d] offset: %#lx", i, phdr->p_offset);
		DBG("phdr[%d] filesz: %#lx", i, phdr->p_filesz);
		if (phdr->p_offset >= position)
		{
			DBG("phdr[%d] shifting by %zu since its after (%#lx)", i, size,
				phdr->p_offset);
			phdr->p_offset += size;
		}
		else if (phdr->p_offset + phdr->p_filesz >= position)
		{
			DBG("phdr[%d] growing by %zu since its final pos is after (%#lx)", i, size,
				position);
			phdr->p_filesz += size;
			phdr->p_memsz += size;
		}
		i++;
	}
}

FASTCALL void
Func(elfstream_section_offset)(t_elfstream *stream, size_t size, size_t position)
{
	size_t		i;
	Elf(Shdr)	*shdr;

	i = 0;
	DBG("injecting code (%#lx) at %#lx", size, position);
	while (i < stream->section_count)
	{
		shdr = (Elf(Shdr) *) &stream->sections[i].shdr32;
		DBG("offsetting shdr[%d]: %p", i, shdr);
		DBG("shdr[%d] offset: %#lx", i, shdr->sh_offset);
		DBG("shdr[%d] size: %#lx", i, shdr->sh_size);
		if (shdr->sh_offset >= position)
		{
			DBG("shdr[%d] shifting by %zu since its after (%#lx)", i, size,
				shdr->sh_offset);
			shdr->sh_offset += size;
		}
		else if (shdr->sh_offset + shdr->sh_size >= position)
		{
			DBG("shdr[%d] growing by %zu since its final pos is after (%#lx)", i, size,
				position);
			shdr->sh_size += size;
		}
		i++;
	}
}

FASTCALL size_t
Func(update_stream)(t_elfstream *stream, t_elf_segment *segment, size_t size)
{
	Elf(Phdr)	*phdr;
	size_t		position;
	size_t		pos_virt;
	Elf(Ehdr)	*ehdr;

	phdr = (Elf(Phdr) *) &segment->phdr32;
	if (size == 0)
		return (phdr->p_filesz);
	DBG("size start: %#lx", phdr->p_filesz);
	position = phdr->p_offset + phdr->p_filesz;
	ehdr = (Elf(Ehdr) *) &stream->ehdr32;
	Func(elfstream_segment_offset)(stream, size, position);
	ehdr->e_shoff += size;
	pos_virt = phdr->p_vaddr + phdr->p_memsz + size; 
	if (pos_virt < ehdr->e_entry)
		ehdr->e_entry += size;
	Func(elfstream_section_offset)(stream, size, position);
	DBG("size end: %#lx", phdr->p_filesz);
	// phdr->p_filesz += size;
	// phdr->p_memsz += size;
	return (phdr->p_filesz - size);
}

# undef ELF_BITNESS
#endif // ELF_BITNESS
