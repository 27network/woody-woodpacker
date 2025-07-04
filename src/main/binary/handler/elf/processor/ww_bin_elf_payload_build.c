/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_build.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/04 20:49:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Warning to whomever reads this:
//
// This junk of letters and numbers someone would've once called "code" was
// written by someone at the end of the world.  It's an amalgamation of
// hacks, dirty hacks, and shitty dirty hacks.
//
// This file makes me genuinely want to gouge my eyeballs out of my fucking
// skull.  I'm deeply and utterly sorry to anyone that should even consider
// attempting to read this.

#include "elfstream.h"
#include <elf.h>
#include <fcntl.h>
#include <ft/io.h>
#include <ft/mem.h>
#include <ft/string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ww/binary/elf.h>

#ifndef ELF_BITNESS 
# define ELF_BITNESS 32
# include "ww_bin_elf_payload_build.c"
# define ELF_BITNESS 64
# include "ww_bin_elf_payload_build.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

/**
 * The structure that holds the payload features.
 *
 * @note	This structure references the entry payload variables directly,
 * 			see `src/shellcode/elf/common/entry/variables.inc.s`.
 */
struct Func(s_payload_features)
{
	Elf(Off)	start_offset;
	Elf(Off)	decryption_routine_offset;
	Elf(Off)	decompression_routine_offset;
	char		encryption_key[16];
	char		loader_async;
	char		_padding2[7];
	Elf(Off)	user_payload_size;
	Elf(Off)	segments_write_offset;
	Elf(Off)	segments_content_size;
};

#define SIZE_SIZE sizeof(Elf(Off))

static_assert(sizeof(struct Func(s_payload_features)) == (3 * SIZE_SIZE) + 24 + (3 * SIZE_SIZE),
	"struct Func(s_payload_features) is not what we expected");

#undef SIZE_SIZE

FASTCALL Elf(Off)	Func(ww_bin_elf_entry_offset)(t_ww_elf_handler *self, Elf(Off) woody_entry)
{
	Elf(Ehdr)	*ehdr;

	ehdr = (Elf(Ehdr) *) &self->stream.ehdr32;
	return (ehdr->e_entry - woody_entry);
}

FASTCALL bool	Func(ww_bin_read_content)(
	t_elf_segment *segment,
	char *segment_buffer
) {
	static long			page_size = 4096; // because fuck you, we dont have sysconf(3).
	t_content_source	*content_source = segment->content;

	while (content_source)
	{
		if (content_source->type == CONTENT_SOURCE_FILE)
		{
			int		fd = content_source->s_file.fd;
			off_t	offset = content_source->s_file.offset;
			off_t	aligned_offset = offset & ~(page_size - 1);
			off_t	diff_offset = offset - aligned_offset;
			off_t	map_size = content_source->size + diff_offset;

			char	*buffer = mmap(NULL, map_size, PROT_READ, MAP_PRIVATE, fd, aligned_offset);
			if (buffer == MAP_FAILED)
				return (false);
			ft_memcpy(segment_buffer, buffer + diff_offset, content_source->size);
			munmap(buffer, map_size);
		}
		else if (content_source->type == CONTENT_SOURCE_MEMORY)
			ft_memcpy(segment_buffer, content_source->s_memory.data, content_source->size);
		content_source = content_source->next;
	}
	elfstream_segment_shrink(stream, segment);
	// elfstream_content_free(segment->content);
	// segment->content = NULL;
	// Elf(Phdr)	*phdr = (Elf(Phdr) *)&segment->phdr32;
	// phdr->p_filesz = 0;
	return (true);
}

FASTCALL char	*Func(ww_bin_get_segments_content)(
	t_elfstream *stream,
	Elf(Off) *segments_write_offset,
	Elf(Off) *segments_content_size,
	Elf(Off) woody_entry
) {
	t_elf_segment	*target = NULL;
	Elf(Phdr)		*phdr = NULL;
	size_t			i;

	i = 0;
	while (i < stream->segment_count)
	{
		t_elf_segment *tmp = &stream->segments[i];
		phdr = (Elf(Phdr) *)&tmp->phdr32;
		if (phdr->p_type == PT_LOAD && (phdr->p_flags & PF_X) == PF_X)
		{
			target = tmp;
			break ;
		}
		i++;
	}

	if (!target)
	{
		ww_warn("No segments with PF_X found, no executable content???\n");
		return (NULL);
	}
	size_t address = phdr->p_vaddr;
	*segments_write_offset = woody_entry - address;

	*segments_content_size = elfstream_content_size(target->content);
	char *segments_content = ft_calloc(*segments_content_size, sizeof(char));
	if (!segments_content)
		return (NULL);
	if (!Func(ww_bin_read_content)(target, segments_content))
		ft_strdel(&segments_content);
	return (segments_content);
}

// FASTCALL char	*Func(ww_bin_get_segments_content)(
// 	t_elfstream *stream,
// 	Elf(Off) *segments_write_offset,
// 	Elf(Off) *segments_content_size,
// 	Elf(Off) woody_entry
// ) {
// 	Elf(Phdr)		*phdr = NULL;
// 	Elf(Phdr)		*last_phdr = NULL;
// 	t_elf_segment	*segments = stream->segments;
// 	t_elf_segment	*last_valid = NULL;
// 	int				i = stream->segment_count - 1;
// 	Elf(Off)		start_address = 0;
//
// 	while (i >= 0)
// 	{
// 		phdr = (Elf(Phdr) *)&segments[i].phdr32;
// 		i--;
//
// 		ww_trace("considering segment %zu for contig check\n", i + 1);
// 		if (phdr->p_type != PT_LOAD)
// 			continue;
// 		ww_trace("> is PT_LOAD\n");
//
// 		if (last_valid)
// 		{
// 			last_phdr = (Elf(Phdr) *) &last_valid->phdr32;
// 			/* CHECK IF SEGMENTS NOT CONTIGUOUS */
// 			if (phdr->p_vaddr + phdr->p_memsz != last_phdr->p_vaddr)
// 			{
// 				ww_trace("segment %zu is not contiguous with last\n", i + 1);
// 				i += 2;
// 				break ;
// 			}
// 			ww_trace("segment %zu is contiguous with last\n", i + 1);
// 		}
// 		
// 		*segments_content_size += phdr->p_memsz;
// 		start_address = phdr->p_vaddr;
//
// 		last_valid = &segments[i + 1];
// 	}
// 	if (i < 0)
// 		i = 0;
// 	ww_trace("start_address: %#lx\n", (size_t)start_address);
// 	*segments_write_offset = woody_entry - start_address;
// 	ww_trace("segments write offset: %#lx\n", (size_t) *segments_write_offset);
// 	ww_trace("segments content size: %lu\n", (size_t) *segments_content_size);
// 	
// 	/* WRITE CONTENT TO SEGMENTS_CONTENT */
// 	Elf(Off)	offset = 0;
// 	bool		error = false;
// 	char		*segments_content = ft_calloc(*segments_content_size, sizeof(char));
// 	if (!segments_content)
// 		return NULL;
// 	while (i < stream->segment_count && !error)
// 	{
// 		last_valid = &segments[i];
// 		phdr = (Elf(Phdr) *)&last_valid->phdr32;
// 		if (phdr->p_type == PT_LOAD)
// 		{
// 			error |= !Func(ww_bin_read_content)(last_valid, segments_content + offset);
// 			offset += phdr->p_memsz;
// 		}
// 		i++;
// 	}
// 	if (error)
// 		ft_strdel(&segments_content);
// 	return (segments_content);
// }

/**
 * Cette fonction build le payload final selon:
 * - L'algo de décompression
 * - L'algo de décryption
 * - L'entrypoint (entrypoint.s)
 *   - Contient tout (entry, call decompress, call decrypt, loader)
 *   - + Variables à la fin
 */
t_content_source *Func(ww_bin_elf_payload_build)(
	t_ww_binary *bin,
	t_ww_elf_handler *self,
	[[maybe_unused]] t_elf_segment *segment,
	Elf(Off) woody_entry,
	Elf(Off) *routines_offset
) {
	struct Func(s_payload_features)	features;
	ft_memset(&features, 0, sizeof(features));
	features.loader_async = bin->args->payload_async;

	// smartstr segments_content_compressed = NULL;

	// Setup our dynamic buffers (segments & user payload)
	smartstr segments_content = Func(ww_bin_get_segments_content)(
		&self->stream,
		&features.segments_write_offset,
		&features.segments_content_size,
		woody_entry
	);
	ww_trace("found segments content, size: %lu\n", (size_t) features.segments_content_size);
	if (!segments_content && features.segments_content_size != 0)
		return (NULL);
	smartstr user_payload = Func(ww_bin_elf_payload_user)(bin, &features.user_payload_size);
	ww_trace("found user payload, size: %lu\n", (size_t) features.user_payload_size);
	if (!user_payload && features.user_payload_size)
		return (NULL);

	// Note: This size is the size of the payload, minus the user-defined extra content (user_payload + segments_content)
	Elf(Off) payload_size = 0;
	Elf(Off) user_content_size = features.user_payload_size + features.segments_content_size;
	ww_trace("user content size: %lu\n", (size_t) user_content_size);
	// Allocate payload from the raw payload (inc decrypt/decompress routines + entry),
	// and add user payload and segments content size.
	char *payload = Func(ww_bin_elf_payload_raw)(bin, routines_offset, &payload_size, user_content_size);
	if (!payload)
		return (NULL);
	woody_entry += *routines_offset;
	// aes-128();
	// smlz_compress(segments_content, features.segments_content_size, segments_content_compressed);

	char *target = payload + payload_size - sizeof(features);
	features.start_offset = Func(ww_bin_elf_entry_offset)(self, woody_entry);
	// features.decryption_routine_offset = 0x2222222222222222;
	// features.decompression_routine_offset = 0x3333333333333333;
	ft_memcpy(features.encryption_key, bin->args->encryption_key, 16);
	features.loader_async = bin->args->payload_async;

	ft_memcpy(target, &features, sizeof(features));
	target = payload + payload_size;
	ww_trace("writing segments content (%#lx bytes at %#lx)\n", (size_t)features.segments_content_size, (size_t)payload_size);
	ft_memcpy(target, segments_content, features.segments_content_size);
	target += features.segments_content_size;
	ww_trace("writing user payload (%#lx bytes at %#lx)\n", (size_t)features.user_payload_size, (size_t)payload_size + features.segments_content_size);
	ft_memcpy(target, user_payload, features.user_payload_size);
	ww_trace("done writing payload\n");

#ifdef WW_DEBUG
	int fd = open("woody-payload.bin", O_WRONLY | O_CREAT, 0755);
	if (fd == -1)
		return (NULL);
	size_t target_write = payload_size + user_content_size;
	size_t write_offset = 0;
	while (target_write)
	{
		ssize_t wrote = write(fd, payload + write_offset, target_write - write_offset);
		if (wrote == -1)
		{
			ww_error("debug write error: %m\n");
			break ;
		}
		target_write -= wrote;
		write_offset += wrote;
	}
	close(fd);
#endif

	t_content_source *source = elfstream_source_data(payload, payload_size + user_content_size, true);
	if (!source)
		free(payload);
	return (source);
}

# undef ELF_BITNESS
#endif
