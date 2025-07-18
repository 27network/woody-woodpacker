/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:37:16 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/18 11:44:14 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <ft/string.h>
#include <stdint.h>
#include <unistd.h>
#include <ww/binary.h>

static inline void	ww_bin_key_hash(char out[16], const char *in_str)
{
	const unsigned char *in = (const unsigned char *) in_str;
	const size_t in_size = ft_strlen(in_str);
	ft_bzero(out, 16);

	const uint32_t prime1 = 0x9E3779B1;
    const uint32_t prime2 = 0x85EBCA77;
    const uint32_t prime3 = 0xC2B2AE3D;
    const uint32_t prime4 = 0x27D4EB2F;
	uint32_t h1 = prime1;
	uint32_t h2 = prime2;
	uint32_t h3 = prime3;
	uint32_t h4 = prime4;

	for (size_t i = 0; i < in_size; i++)
	{
		unsigned char byte = in[i];
		h1 = ((h1 << 7) | (h1 >> 25)) ^ byte;
        h1 *= prime1;
        h2 = ((h2 << 11) | (h2 >> 21)) ^ byte;
        h2 *= prime2;
        h3 = ((h3 << 13) | (h3 >> 19)) ^ byte;
        h3 *= prime3;
        h4 = ((h4 << 17) | (h4 >> 15)) ^ byte;
        h4 *= prime4;
		h1 ^= i * prime2;
        h2 ^= i * prime3;
        h3 ^= i * prime4;
        h4 ^= i * prime1;
	}

	h1 ^= in_size;
    h2 ^= in_size;
    h3 ^= in_size;
    h4 ^= in_size;
    
    h1 ^= h1 >> 16;
    h1 *= prime3;
    h1 ^= h1 >> 13;
    h1 *= prime4;
    h1 ^= h1 >> 16;
    h2 ^= h2 >> 16;
    h2 *= prime4;
    h2 ^= h2 >> 13;
    h2 *= prime1;
    h2 ^= h2 >> 16;
    h3 ^= h3 >> 16;
    h3 *= prime1;
    h3 ^= h3 >> 13;
    h3 *= prime2;
    h3 ^= h3 >> 16;
    h4 ^= h4 >> 16;
    h4 *= prime2;
    h4 ^= h4 >> 13;
    h4 *= prime3;
    h4 ^= h4 >> 16;

	ww_debug("Encryption key hash: '%x%x%x%x'\n", h1, h2, h3, h4);

	ft_memcpy(out, &h1, 4);
	ft_memcpy(out + 4, &h2, 4);
	ft_memcpy(out + 8, &h3, 4);
	ft_memcpy(out + 12, &h4, 4);
}

static const char g_charset[] = "0123456789abcdef";
static const size_t g_charset_size = sizeof(g_charset) - 1;

static inline t_ww_error	ww_bin_key_random(char *key, size_t key_size)
{
	smartfd fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (ww_err_fmt(ERROR_INTERNAL, "failed to open /dev/urandom"));

	size_t i = 0;
	while (i < key_size)
	{
		unsigned char c;
		if (read(fd, &c, 1) != 1)
			return (ww_err_fmt(ERROR_INTERNAL, "failed to read from /dev/urandom"));

		if (c < g_charset_size)
			key[i++] = g_charset[c];
	}
	return (ww_ok());
}

/**
 * This function should guarentee that:
 * - The encryption key is set
 *   - If not, it will generate a random one, ideally human-readable
 * - The encryption key is 16 bytes long
 *   - For this, it will use a hashing function to reduce the key to 16 bytes
 */
static inline t_ww_error	ww_bin_keygen(t_ww_binary *bin)
{
	t_ww_error	err;

	err = ww_ok();
	ww_trace("Raw encryption key: '%s'\n", bin->args->encryption_key);
	if (bin->args->encryption_key == NULL)
	{
		ww_trace("Encryption key was not provided, generating a random one\n");
		bin->args->encryption_key = ft_calloc(65, 1);
		if (!bin->args->encryption_key)
			return (ww_err_fmt(ERROR_ALLOC, "failed to allocate pre-op encryption key"));
		bin->args->encryption_key_alloc = true;
		err = ww_bin_key_random(bin->args->encryption_key, 64);
		if (err.type)
			return (err);
	}
	ww_info("Encryption key: '%s'\n", bin->args->encryption_key);
	
	char tmp[17];
	ft_memset(tmp, 0, 17);
	ww_bin_key_hash(tmp, bin->args->encryption_key);
	if (bin->args->encryption_key_alloc)
		ft_strdel(&bin->args->encryption_key);
	bin->args->encryption_key_alloc = true;
	bin->args->encryption_key = ft_calloc(16 + 240, 1);
	if (!bin->args->encryption_key)
		return (ww_err_fmt(ERROR_ALLOC, "failed to allocate post-op encryption key"));
	ft_memcpy(bin->args->encryption_key, tmp, 17);
	return (err);
}

t_ww_error	ww_bin_process(t_ww_binary *bin)
{
	t_ww_error	err;

	err = ww_bin_keygen(bin);
	if (err.type)
		return (err);
	if (bin && bin->handler && bin->handler->process)
	{
		ww_trace("Processing %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->process(bin->handler, bin));
	}
	return (ww_err_fmt(ERROR_INTERNAL, "No 'process' handler for %s\n",
			bin->input));
}
