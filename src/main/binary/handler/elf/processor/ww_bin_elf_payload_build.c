/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_build.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 11:54:00 by kiroussa         ###   ########.fr       */
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
	[[maybe_unused]] t_ww_elf_handler *self,
	[[maybe_unused]] t_elf_segment *segment,
	Elf(Off) woody_entry,
	Elf(Off) *routines_offset
) {
	struct Func(s_payload_features)	features;
	ft_memset(&features, 0, sizeof(features));
	features.loader_async = bin->args->payload_async;

	smartstr segments_content = NULL;
	smartstr user_payload = NULL;

	// Setup our dynamic buffers (segments & user payload)
	segments_content = NULL; //TODO: Implement
	ww_trace("found segments content, size: %lu\n", features.segments_content_size);
	if (!segments_content && features.segments_content_size != 0)
		return (NULL);
	user_payload = Func(ww_bin_elf_payload_user)(bin, &features.user_payload_size);
	ww_trace("found user payload, size: %lu\n", features.user_payload_size);
	if (!user_payload && features.user_payload_size)
		return (NULL);

	// Note: This size is the size of the payload, minus the user-defined extra content (user_payload + segments_content)
	Elf(Off) payload_size = 0;
	Elf(Off) user_content_size = features.user_payload_size + features.segments_content_size;
	// Allocate payload from the raw payload (inc decrypt/decompress routines + entry),
	// and add user payload and segments content size.
	char *payload = Func(ww_bin_elf_payload_raw)(bin, routines_offset, &payload_size, user_content_size);
	if (!payload)
		return (NULL);
	woody_entry += *routines_offset;

	char *target = payload + payload_size - sizeof(features);
	features.start_offset = Func(ww_bin_elf_entry_offset)(self, woody_entry);
	features.decryption_routine_offset = 0x2222222222222222;
	features.decompression_routine_offset = 0x3333333333333333;
	ft_memcpy(features.encryption_key, "42424242424242424242424242424242", 16);
	features.loader_async = bin->args->payload_async;
	features.segments_write_offset = 0x5555555555555555;
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
	write(fd, payload, payload_size);
	close(fd);
#endif

	t_content_source *source = elfstream_source_data(payload, payload_size + user_content_size, true);
	if (!source)
		free(payload);
	return (source);
}

# undef ELF_BITNESS
#endif
