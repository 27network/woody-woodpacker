/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_build.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/18 18:36:03 by kiroussa         ###   ########.fr       */
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
#define _SMLZ_IMPL
#include <ww/compress/smlz.h>
#include <ww/encrypt/aes128.h>

#ifndef ELF_BITNESS 

typedef struct {
    uint64_t rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
} register_state_t;

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

static_assert(
	sizeof(struct Func(s_payload_features)) == (3 * sizeof(Elf(Off))) + 24 + (3 * sizeof(Elf(Off))),
	"struct Func(s_payload_features) is not what we expected"
);

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
	size_t size = elfstream_content_size(segment->content);
	elfstream_content_free(segment->content);
	segment->content = elfstream_source_data(NULL, size, false);

	//TODO: Shrinking
	// elfstream_segment_shrink(stream, segment);
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
			phdr->p_flags |= PF_W;
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
	ww_trace("Stealing shit from %p\n", (void *)address);
	*segments_write_offset = address - woody_entry;

	*segments_content_size = elfstream_content_size(target->content);
	char *segments_content = ft_calloc(*segments_content_size, sizeof(char));
	if (!segments_content)
		return (NULL);
	if (!Func(ww_bin_read_content)(target, segments_content))
		ft_strdel(&segments_content);
	return (segments_content);
}

FASTCALL bool	Func(ww_smlz_compress)(
	char *source,
	Elf(Off) *source_size,
	char **target
) {
	ww_trace("smlz compressing buffer (total: %lu)\n", (size_t) *source_size);
	t_smlz_buffer input_buffer = (t_smlz_buffer){source, *source_size, 0};
	t_smlz_buffer output_buffer = (t_smlz_buffer){NULL, 0, 0};
	ww_info("Compressing ELF code, this may take a while... (1/2)\n");
	smlz_compress_impl(&input_buffer, &output_buffer, 8);
	if (output_buffer.offset > *source_size)
	{
		ww_warn("Compressed size is greater than source size\n");
		return (false);
	}
	input_buffer.offset = 0;
	output_buffer.data = ft_calloc(output_buffer.offset + 10, sizeof(char));
	if (!output_buffer.data)
	{
		ww_error("failed to allocate output buffer\n");
		return (false);
	}
	ww_info("Compressing ELF code, this may take a while... (2/2)\n");
	smlz_compress_impl(&input_buffer, &output_buffer, 8);
	ft_strdel(target);
	*source_size = ((Elf(Off)) output_buffer.offset);
	ww_trace("compressed via smlz (total: %lu)\n", (size_t) *source_size);
	*target = output_buffer.data;
	return (true);
}

FASTCALL void	Func(ww_aesify)(
	t_ww_binary *bin,
	char *plaintext,
	Elf(Off) plaintext_size
) {
	ww_trace("aesifying buffer (total: %lu)\n", (size_t) plaintext_size);
	Elf(Off) done = 0;
	while (done < plaintext_size && plaintext_size - done >= 16)
	{
		aes128_encrypt(plaintext + done, bin->args->encryption_key);
		done += 16;
	}
	ww_trace("done aesifying buffer (wrote %lu bytes)\n", (size_t) done);
}

/**
 *
 */
FASTCALL char	*Func(ww_bin_elf_payload_process)(
	t_ww_binary *bin,
	[[maybe_unused]] t_ww_elf_handler *handler,
	char *segments_content,
	Elf(Off) *segments_content_size
) {
	if (!segments_content || *segments_content_size == 0)
		return (NULL);

	char *temp_buffer = ft_calloc(*segments_content_size + 1, sizeof(char));
	if (!temp_buffer)
	{
		ww_error("failed to allocate temp buffer for segments content processing\n");
		return (NULL);
	}
	switch (bin->args->compression_algo)
	{
		case COMPRESSION_ALGO_SMLZ:
			if (!Func(ww_smlz_compress)(segments_content, segments_content_size, &temp_buffer))
			{
				ww_warn("SMLZ compression failed, falling back to uncompressed\n");
				bin->args->compression_algo = COMPRESSION_ALGO_NONE;
				ft_memcpy(temp_buffer, segments_content, *segments_content_size);
			}
			break ;
		default:
			ft_memcpy(temp_buffer, segments_content, *segments_content_size);
			break ;
	}

	// Swap them
	ft_strdel(&segments_content);
	segments_content = temp_buffer;
	temp_buffer = NULL;

	switch (bin->args->encryption_algo)
	{
		case ENCRYPTION_ALGO_AES128:
			Func(ww_aesify)(bin, segments_content, *segments_content_size);
			break ;
		default:
			break ;
	}

	return (segments_content);
}

/**
 * This function builds the final payload with:
 * - Decompression routine
 * - Decryption routine
 * - Woody entrypoint (entrypoint.s)
 *   - Contains all (entry, call decompress, call decrypt, loader)
 *     - w/ variables.inc.s at the end 
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

	// Setup our dynamic buffers (segments & user payload)
	smartstr segments_content = Func(ww_bin_get_segments_content)(
		&self->stream,
		&features.segments_write_offset,
		&features.segments_content_size,
		woody_entry
	);
	ww_trace("found segments content, size: %lu\n", (size_t) features.segments_content_size);
	if (features.segments_content_size > 0 && segments_content)
	{
		segments_content = Func(ww_bin_elf_payload_process)(
			bin,
			self,
			segments_content,
			&features.segments_content_size
		);
		ww_trace("processed segments content, new size: %lu\n", (size_t) features.segments_content_size);
	}
	if (!segments_content && features.segments_content_size != 0)
		return (NULL);
	ww_trace("seeking user payload\n");
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
	char *payload = Func(ww_bin_elf_payload_raw)(
		bin,
		routines_offset,
		&features.decryption_routine_offset,
		&features.decompression_routine_offset,
		&payload_size,
		user_content_size
	);
	if (!payload)
		return (NULL);
	woody_entry += *routines_offset;
	features.segments_write_offset -= *routines_offset;

	char *target = payload + payload_size - sizeof(features);
	features.start_offset = Func(ww_bin_elf_entry_offset)(self, woody_entry);
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

