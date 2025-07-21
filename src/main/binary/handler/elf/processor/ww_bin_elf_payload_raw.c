/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_raw.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:25:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/21 23:03:59 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/elf.h>
#include <ww/compress.h>
#include <ww/encrypt.h>

#ifndef ELF_BITNESS
# define ELF_BITNESS 32
# define DECOMPRESS_NONE_FILE "src/shellcode/elf/x86/decompress/none.bin"
# define DECOMPRESS_SMLZ_FILE "src/shellcode/elf/x86/decompress/smlz.bin"
# define DECRYPT_AES_FILE "src/shellcode/elf/x86/decrypt/aes128.bin"
# define DECRYPT_NONE_FILE "src/shellcode/elf/x86/decrypt/none.bin"
# define PAYLOAD_FILE "src/shellcode/elf/x86/entry/entrypoint.bin"
# include "ww_bin_elf_payload_raw.c"

# define ELF_BITNESS 64
# define DECOMPRESS_NONE_FILE "src/shellcode/elf/x86_64/decompress/none.bin"
# define DECOMPRESS_SMLZ_FILE "src/shellcode/elf/x86_64/decompress/smlz.bin"
# define DECRYPT_AES_FILE "src/shellcode/elf/x86_64/decrypt/aes128.bin"
# define DECRYPT_NONE_FILE "src/shellcode/elf/x86_64/decrypt/none.bin"
# define PAYLOAD_FILE "src/shellcode/elf/x86_64/entry/entrypoint.bin"
# include "ww_bin_elf_payload_raw.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

static const char	Func(g_payload)[] = {
# embed PAYLOAD_FILE
};

static const char	Func(g_decompress_bincode_none)[] = {
# embed DECOMPRESS_NONE_FILE
};

static const char	Func(g_decompress_bincode_smlz)[] = {
# embed DECOMPRESS_SMLZ_FILE
};

static const char	Func(g_decrypt_bincode_aes)[] = {
# embed DECRYPT_AES_FILE
};

static const char	Func(g_decrypt_bincode_none)[] = {
# embed DECRYPT_NONE_FILE
};

FASTCALL const char	*Func(ww_bin_elf_payload_decompress)(t_ww_binary *bin, Elf(Off) *size)
{
	*size = 0;
	ww_debug("compression algo: %s\n", ww_compression_algo_str(bin->args->compression_algo));
	if (bin->args->compression_algo == COMPRESSION_ALGO_NONE)
	{
		*size = sizeof(Func(g_decompress_bincode_none));
		return (Func(g_decompress_bincode_none));
	}
	if (bin->args->compression_algo == COMPRESSION_ALGO_SMLZ)
	{
		*size = sizeof(Func(g_decompress_bincode_smlz));
		return (Func(g_decompress_bincode_smlz));
	}
	ww_warn("unimplemented compression algorithm: %s\nwill not compress segments\n",
		ww_compression_algo_str(bin->args->compression_algo));
	return (NULL);
}

FASTCALL const char *Func(ww_bin_elf_payload_decrypt)(t_ww_binary *bin, Elf(Off) *size)
{
	*size = 0;
	ww_debug("encryption algo: %s\n", ww_encryption_algo_str(bin->args->encryption_algo));
	if (bin->args->encryption_algo == ENCRYPTION_ALGO_AES128)
	{
		*size = sizeof(Func(g_decrypt_bincode_aes));
		return (Func(g_decrypt_bincode_aes));
	}
	if (bin->args->encryption_algo == ENCRYPTION_ALGO_NONE)
	{
		*size = sizeof(Func(g_decrypt_bincode_none));
		return (Func(g_decrypt_bincode_none));
	}
	ww_warn("unimplemented encryption algorithm: %s\nwill not encrypt segments\n",
		ww_encryption_algo_str(bin->args->encryption_algo));
	return (NULL);
}

char	*Func(ww_bin_elf_payload_raw)(
	t_ww_binary *bin,
	Elf(Off) *routines_offset,
	Elf(Off) *decryption_offset,
	Elf(Off) *decompression_offset,
	Elf(Off) *initial_size,
	Elf(Off) offset
) {
	Elf(Off) decrypt_size = 0;
	const char *decrypt = Func(ww_bin_elf_payload_decrypt)(bin, &decrypt_size);
	ww_trace("decrypt_size: %#lx\n", (size_t)decrypt_size);

	Elf(Off) decompress_size = 0;
	const char *decompress = Func(ww_bin_elf_payload_decompress)(bin, &decompress_size);
	ww_trace("decompress_size: %#lx\n", (size_t)decompress_size);
	
	*decryption_offset = -(decrypt_size + decompress_size);
	ww_trace("decryption_offset: %#lx (%ld)\n", (size_t)*decryption_offset, (int64_t) *decryption_offset);
	*decompression_offset = -decompress_size;
	ww_trace("decompression_offset: %#lx (%ld)\n", (size_t)*decompression_offset, (int64_t) *decompression_offset);
	*routines_offset = decompress_size + decrypt_size;
	ww_trace("routines_offset: %#lx (%ld)\n", (size_t)*routines_offset, (int64_t) *routines_offset);

	// This will account for the entirety of the payload, minus the user-defined extra content...
	ww_trace("payload_raw_size: %#lx\n", (size_t)sizeof(Func(g_payload)));
	Elf(Off) payload_size = sizeof(Func(g_payload)) + *routines_offset;
	// (Since variables.inc.s (end of payload) has a two-sys-word stub at the end, we need to remove it)
	Elf(Off) payload_stub_size = sizeof(Elf(Off)) * 2;
	payload_size -= payload_stub_size;
	ww_trace("payload_initial_size: %#lx\n", (size_t)payload_size);
	*initial_size = payload_size;
	ww_trace("user_offset: %#lx\n", (size_t)offset);
	
	// Allocation :pray:
	ww_trace("payload_size: %#lx\n", (size_t)payload_size);
	char *payload = ft_calloc(payload_size + offset, 1);
	ww_trace("payload: %p\n", (void *)payload);

	// Write the initial static payload data
	if (payload)
	{
		char *target = payload;
		ww_trace("writing decrypt routine (%#lx/%#lx)\n", (size_t)decrypt_size, (size_t)payload_size);
		ft_memcpy(target, decrypt, decrypt_size);
		target += decrypt_size;
		ww_trace("writing decompress routine (%#lx/%#lx)\n", (size_t)decrypt_size + (size_t)decompress_size, (size_t)payload_size);
		ft_memcpy(target, decompress, decompress_size);
		target += decompress_size;
		ww_trace("writing payload (%#lx/%#lx)\n", (size_t)decompress_size + (size_t)decrypt_size + (size_t)(sizeof(Func(g_payload)) - payload_stub_size), (size_t)payload_size);
		// Note that this write contains the variables.inc.s content, but not rewritten yet
		ft_memcpy(target, Func(g_payload), sizeof(Func(g_payload)) - payload_stub_size);
		ww_trace("done writing raw payload\n");
	}

#ifdef WW_DEBUG
	int fd = open("woody-payload.raw.bin", O_WRONLY | O_CREAT, 0755);
	if (fd == -1)
		return (NULL);
	(void)!write(fd, payload, payload_size);
	close(fd);
#endif

	return (payload);
}

# undef DECOMPRESS_NONE_FILE
# undef DECOMPRESS_SMLZ_FILE
# undef DECRYPT_NONE_FILE
# undef DECRYPT_AES_FILE
# undef PAYLOAD_FILE
# undef ELF_BITNESS
#endif
