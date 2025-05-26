/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload.inc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/26 21:53:32 by kiroussa         ###   ########.fr       */
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

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <ft/io.h>
#include <ft/mem.h>
#include <ft/string.h>

#if ELF_BITNESS == 32
# define SRC_PREFIX "src/shellcode/elf/x86"
#elif ELF_BITNESS == 64
# define SRC_PREFIX "src/shellcode/elf/x86_64"
#else
# error "Invalid ELF_BITNESS"
#endif

# define PAYLOAD_FILE SRC_PREFIX "/entry/entrypoint.bin"
# define USER_PAYLOAD_FILE SRC_PREFIX "/payload/payload.bin"
# define DECRYPT_AES_FILE SRC_PREFIX "/decrypt/aes.bin"
# define DECRYPT_XOR_FILE SRC_PREFIX "/decrypt/xor.bin"
# define DECOMPRESS_SMLZ_FILE SRC_PREFIX "/decompress/smlz.bin"

static const char	Func(g_payload)[] = {
#embed PAYLOAD_FILE 
}

static const char	Func(g_default_user_payload)[] = {
#embed USER_PAYLOAD_FILE 
};

static const char	Func(g_decrypt_bincode_aes)[] = {
#embed DECRYPT_AES_FILE
};

static const char	Func(g_decrypt_bincode_xor)[] = {
#embed DECRYPT_XOR_FILE
};

static const char	Func(g_decompress_bincode_smlz)[] = {
#embed DECOMPRESS_SMLZ_FILE
};

#undef DECOMPRESS_SMLZ_FILE
#undef DECRYPT_XOR_FILE
#undef DECRYPT_AES_FILE
#undef USER_PAYLOAD_FILE
#undef PAYLOAD_FILE
#undef SRC_PREFIX

/**
 * The structure that holds the payload features.
 *
 * @note	This structure references the entry payload variables directly,
 * 			see `src/shellcode/elf/common/entry/variables.inc.s`.
 */
__attribute__((packed))
struct Func(s_payload_features)
{
	Elf(Off)	start_offset;
	Elf(Off)	decryption_routine_offset;
	Elf(Off)	decompression_routine_offset;
	char		encryption_key[16];
	char		loader_async;
	Elf(Off)	user_payload_size;
	Elf(Off)	segments_offset;
	Elf(Off)	segments_content_size;
};

FASTCALL char	*Func(ww_bin_elf_payload_raw)(
	Elf(Off) *total_size,
	Elf(Off) *orig_size,
	Elf(Off) segments_size,
	Elf(Off) payload_size
) {
	char		*payload;
	Elf(Off)	payload_size;

	payload_size = sizeof(Func(g_payload));
	// We need to deduct 2 Elf(Off) from the payload size
	// to account for `payload` and `segments_content`
	payload_size -= sizeof(Elf(Off)) * 2;
	*orig_size = payload_size;
	// And then add the actual size of those buffers
	payload_size += segments_size;
	payload_size += payload_size;

	ww_trace("allocating %#x bytes for payload\n", (unsigned int)payload_size);
	ww_trace("segments_size: %#x\n", (unsigned int)segments_size);
	ww_trace("payload_size: %#x\n", (unsigned int)payload_size);

	*total_size = payload_size;
	return (ft_calloc(payload_size, 1));
}

char	*Func(ww_bin_elf_payload_user)(
	t_ww_binary *bin,
	int *user_payload_fd,
	Elf(Off) *user_payload_size
) {
	char *user_payload;
	Elf(Off) size;

	if (bin->args->payload_file == NULL || ft_strcmp(bin->args->payload_file, "none") == 0)
	{
		*user_payload_size = 0;
		return (NULL);
	}
	if (ft_strcmp(bin->args->payload_file, "<built-in>") == 0)
	{
		*user_payload_size = Func(g_default_user_payload_size);
		user_payload = ft_calloc(*user_payload_size, 1);
		if (user_payload)
			ft_memcpy(user_payload, Func(g_default_user_payload),
				*user_payload_size);
		return (user_payload);
	}
	*user_payload_fd = open(bin->args->payload_file, O_RDONLY);
	if (*user_payload_fd == -1)
	{
		ww_warn("failed to open payload file: %m\n");
		ww_warn("no payload will be used\n");
		*user_payload_size = 0;
		return (NULL);
	}
	struct stat	st;
	if (fstat(*user_payload_fd, &st) != 0)
	{
		ww_warn("failed to fstat payload file: %m\n");
		ww_warn("no payload will be used\n");
		*user_payload_size = 0;
		return (NULL);
	}
	size = (Elf(Off)) st.st_size;
	user_payload = mmap(NULL, size, PROT_READ, MAP_PRIVATE, *user_payload_fd, 0);
	if (user_payload == MAP_FAILED)
	{
		ww_warn("failed to mmap payload file: %m\n");
		ww_warn("no payload will be used\n");
		*user_payload_size = 0;
		return (NULL);
	}
	*user_payload_size = size;
	return (user_payload);
}

t_content_source *Func(ww_bin_elf_payload)(
	t_ww_binary *bin,
	t_ww_elf_handler *self,
	t_elf_segment *segment,
	size_t offset
) {
	__attribute__((cleanup(ft_strdel)))
	char *segments_content;
	Elf(Off) segments_size = 0;

	__attribute__((cleanup(ft_closep)))
	int user_payload_fd = -1;
	__attribute__((cleanup(ft_strdel)))
	char *user_payload;
	Elf(Off) user_payload_size = 0;

	char *payload;
	Elf(Off) orig_payload_size;
	Elf(Off) payload_size;

	// Setup our dynamic buffers (segments & user payload)
	segments_content = NULL; //TODO: Implement
	if (!segments_content && segments_size)
		return (NULL);
	user_payload = Func(ww_bin_elf_payload_user)(bin, &user_payload_fd, &user_payload_size);
	if (!user_payload && user_payload_size)
		return (NULL);

	// Figure out the bincode (decrypt & decompress)
	char *write_ptr = payload;
	char *decrypt_bincode = NULL;
	Elf(Off) decrypt_bincode_size = 0;
	if (!decrypt_bincode)
		ww_warn("unimplemented encryption algorithm: %s\nwill not encrypt segments\n",
			ww_encryption_algo_str(bin->args->encryption_algo));
	char *decompress_bincode = NULL;
	Elf(Off) decompress_bincode_size = 0;
	if (!decompress_bincode)
		ww_warn("unimplemented compression algorithm: %s\nwill not compress segments\n",
			ww_compression_algo_str(bin->args->compression_algo));

	// Allocate full payload
	payload = Func(ww_bin_elf_payload_raw)(&payload_size, &orig_payload_size,
			segments_size, user_payload_size);
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
	memcpy(write_ptr, &features, sizeof(features));
	write_ptr += sizeof(features);

	// Write the dynamic buffers (`segments_content` and `payload`)
	if (features.segments_content_size)
		memcpy(write_ptr, &segments_content, features.segments_content_size);
	write_ptr += features.segments_content_size;
	if (features.user_payload_size)
		memcpy(write_ptr, &user_payload, features.user_payload_size);
	write_ptr += features.user_payload_size;

	return (payload);
}

