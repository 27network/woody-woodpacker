/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_build.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/17 11:05:08 by kiroussa         ###   ########.fr       */
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

#include <ww/binary/elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <ft/io.h>
#include <ft/mem.h>
#include <ft/string.h>

#ifndef ELF_BITNESS 
# define ELF_BITNESS 32
# include "ww_bin_elf_payload_build.c"
# define ELF_BITNESS 64
# include "ww_bin_elf_payload_build.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

// # if ELF_BITNESS == 32
// #  define PAYLOAD_FILE "src/shellcode/elf/x86/entry/entrypoint.bin"
// #  define USER_PAYLOAD_FILE "src/shellcode/elf/x86/payload/payload.bin"
// #  define DECRYPT_AES_FILE "src/shellcode/elf/x86/decrypt/aes.bin"
// #  define DECRYPT_XOR_FILE "src/shellcode/elf/x86/decrypt/xor.bin"
// #  define DECOMPRESS_SMLZ_FILE "src/shellcode/elf/x86/decompress/smlz.bin"
// # elif ELF_BITNESS == 64
// #  define PAYLOAD_FILE "src/shellcode/elf/x86_64/entry/entrypoint.bin"
// #  define DECRYPT_AES_FILE "src/shellcode/elf/x86_64/decrypt/aes.bin"
// #  define DECRYPT_XOR_FILE "src/shellcode/elf/x86_64/decrypt/xor.bin"
// #  define DECOMPRESS_SMLZ_FILE "src/shellcode/elf/x86_64/decompress/smlz.bin"
// # endif
//
// static const char	Func(g_payload)[] = {
// # embed PAYLOAD_FILE
// };
//
// static const char	Func(g_decrypt_bincode_aes)[] = {
// # embed DECRYPT_AES_FILE
// };
//
// static const char	Func(g_decrypt_bincode_xor)[] = {
// # embed DECRYPT_XOR_FILE
// };
//
// static const char	Func(g_decompress_bincode_smlz)[] = {
// #embed DECOMPRESS_SMLZ_FILE
// };
//
// # undef DECOMPRESS_SMLZ_FILE
// # undef DECRYPT_XOR_FILE
// # undef DECRYPT_AES_FILE
// # undef PAYLOAD_FILE
// # undef SRC_PREFIX

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
	Elf(Off)	_padding1;
	char		encryption_key[16];
	char		loader_async;
	char		_padding2[7];
	Elf(Off)	user_payload_size;
	Elf(Off)	segments_offset;
	Elf(Off)	segments_content_size;
};

#define SIZE_SIZE sizeof(Elf(Off))

static_assert(sizeof(struct Func(s_payload_features)) == (4 * SIZE_SIZE) + 24 + (3 * SIZE_SIZE),
	"struct Func(s_payload_features) is not what we expected");

#undef SIZE_SIZE

#define smartstr __attribute__((cleanup(ft_strdel))) char *

/**
 * Cette fonction build le payload final selon:
 * - L'algo de décompression
 * - L'algo de décryption
 * - L'entrypoint (entrypoint.s)
 *   - Contient tout (entry, call decompress, call decrypt, loader)
 *   - + Variables à la fin
 */
t_content_source *Func(ww_bin_elf_payload)(
	[[maybe_unused]] t_ww_binary *bin,
	[[maybe_unused]] t_ww_elf_handler *self,
	[[maybe_unused]] t_elf_segment *segment,
	[[maybe_unused]] size_t offset
) {
	struct Func(s_payload_features)	features;
	ft_memset(&features, 0, sizeof(features));

	smartstr segments_content = NULL;
	smartstr user_payload = NULL;

	// Setup our dynamic buffers (segments & user payload)
	segments_content = NULL; //TODO: Implement
	if (!segments_content && features.segments_content_size != 0)
		return (NULL);
	user_payload = Func(ww_bin_elf_payload_user)(bin, &features.user_payload_size);
	if (!user_payload && features.user_payload_size)
		return (NULL);

	// Setup our payload buffer
	char *payload = NULL;
	Elf(Off) payload_size = 0;
	
	// Allocate payload from the raw payload (inc decrypt/decompress routines + entry),
	// and add user payload and segments content size.
	payload = Func(ww_bin_elf_payload_raw)(&payload_size, features.user_payload_size + features.segments_content_size);
	if (!payload)
		return (NULL);

	// // Figure out the bincode (decrypt & decompress)
	// char				*write_ptr = payload;
	// char 				*decrypt_bincode = NULL;
	// //Elf(Off) 			decrypt_bincode_size = 0;
	//
	// if (!decrypt_bincode)
	// 	ww_warn("unimplemented encryption algorithm: %s\nwill not encrypt segments\n",
	// 		ww_encryption_algo_str(bin->args->encryption_algo));
	// char *decompress_bincode = NULL;
	// //Elf(Off) decompress_bincode_size = 0;
	// if (!decompress_bincode)
	// 	ww_warn("unimplemented compression algorithm: %s\nwill not compress segments\n",
	// 		ww_compression_algo_str(bin->args->compression_algo));

	// Allocate full payload
	payload = Func(ww_bin_elf_payload_raw)();
	if (!payload)
		return (NULL);

	// Write the bincode (TODO: implement)

	// Setup our struct
	struct Func(s_payload_features)	features = {0};
	features.loader_async = bin->args->payload_async,
	features.user_payload_size = user_payload_size;
	features.segments_content_size = segments_size;

	// Write the primary payload features at the end of the payload
	write_ptr = payload + orig_payload_size - sizeof(features);
	ft_memcpy(write_ptr, &features, sizeof(features));
	write_ptr += sizeof(features);

	// Write the dynamic buffers (`segments_content` and `payload`)
	if (features.segments_content_size)
		ft_memcpy(write_ptr, &segments_content, features.segments_content_size);
	write_ptr += features.segments_content_size;
	if (features.user_payload_size)
		ft_memcpy(write_ptr, &user_payload, features.user_payload_size);
	write_ptr += features.user_payload_size;

	t_content_source *source = elfstream_source_data(payload, 0, true);
	if (!source) {
		free(payload);
	}
	return (source);
}

# undef ELF_BITNESS
#endif
