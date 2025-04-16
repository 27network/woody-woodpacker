/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smol_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:45:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/16 18:28:01 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SMOL_MAX_LENGTH		512			// The max lookahead check for flm
#define SMOL_MIN_LENGTH		3			// The min match length for flm
#define SMOL_WINDOW_SIZE	4096		// The window size for flm

#define SMOL_MAGIC			"SMLZ"
#define SMOL_VERSION_MASK	0b00001111
#define SMOL_METADATA_MASK	0b01110000

#define SMOL_FLAG_V1		0b00000001

typedef struct s_sml_header
{
	uint8_t		magic[4];
	uint8_t		flags;
	uint16_t	nblocks;
	uint16_t	block_size;
	uint8_t		remaining;
	uint8_t		reserved[2];
}	t_sml_header;

typedef struct s_sml_buffer
{
	char	*data;
	size_t	size;
	size_t	offset;
}	t_sml_buffer;

size_t	smol_compress(char *in_buf, size_t in_len, char *out_buf);
size_t	smol_decompress(char *in_buf, size_t in_len, char *out_buf);

static inline size_t	min(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

static inline size_t	max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	return (b);
}

static size_t	find_longest_match(
	const uint8_t *data,
	size_t pos,
	size_t data_size,
	uint16_t *length
) {
	const size_t	max_match_length = min(data_size - pos, SMOL_MAX_LENGTH);
	size_t			current_length;
	size_t			i;
	size_t			offset;

	*length = 0;
	if (max_match_length < SMOL_MIN_LENGTH)
		return (0);
	offset = 0;
	i = max(pos - SMOL_WINDOW_SIZE, 0);
	while (i++ < pos)
	{
		current_length = 0;
		while (current_length < max_match_length
			&& data[(i - 1) + current_length] == data[pos + current_length])
			current_length++;
		if (current_length >= SMOL_MIN_LENGTH && current_length > *length)
		{
			*length = current_length;
			offset = pos - (i - 1);
			if (current_length == SMOL_MAX_LENGTH)
				break ;
		}
	}
	return (offset);
}

[[nodiscard]]
static size_t	smol_write_direct(char *buf, size_t offset, void *data,
					size_t len)
{
	if (buf)
		memcpy(buf + offset, data, len);
	return (len);
}

static size_t	smol_write(t_sml_buffer *buf, void *data, size_t len)
{
	const size_t	ret_len
		= smol_write_direct(buf->data, buf->offset, data, len);

	buf->offset += ret_len;
	return (ret_len);
}

static void	smol_block_write(t_sml_buffer *buf, t_sml_buffer *in,
				bool trailing)
{

}

#define SMOL_MIN_DATA_SIZE	

static bool	smol_header_init(t_sml_header *header, t_sml_buffer *in)
{
	size_t	size;

	size = in->size;
	header->flags = SMOL_FLAG_V1;
	if (in->size < 32)
		return (false);
	header->block_size = 8;
	while (size >= 64 && header->block_size < 32768)
	{
		header->block_size *= 2;
		size >>= 2;
	}
	return (true);
}

#define SMOL_BITS 8  // I hope thats not changing soon :pray:

typedef struct s_sml_token
{
	uint16_t	length;
	uint8_t		offset;
}	t_sml_token;

static inline void	smol_compress_block_flip_bit(char *data, char *sml_block, int bit)
{
	if (data)
		sml_block[bit / SMOL_BITS]
			|= (1 << ((SMOL_BITS - (bit + 1)) % SMOL_BITS));
}

static void	smol_compress_block(t_sml_header *header, t_sml_buffer *in,
					t_sml_buffer *out)
{
	const size_t	block_size = header->block_size;
	t_sml_token		token;
	char			*sml_block;
	int				i;

	sml_block = out->data + out->offset;
	out->offset += block_size / SMOL_BITS;
	i = 0;
	while (i++ < block_size)
	{
		token.offset = find_longest_match((uint8_t *)in->data, in->offset,
				in->size, &token.length);
		if (token.length >= SMOL_MIN_LENGTH)
		{
			smol_compress_block_flip_bit(sml_block, i);
			in->offset += token.length;
			smol_write(out, &token, sizeof(t_sml_token));
			continue ;
		}
		in->offset += smol_write(out, in->data + in->offset, 1);
	}
}

static void	smol_compress_blocks(t_sml_header *header, t_sml_buffer *in,
					t_sml_buffer *out)
{
	const size_t	block_size = header->block_size;

	while (in->offset < in->size)
	{
		if (in->offset + block_size > in->size)
		{
			header->remaining = smol_write(out, in->data + in->offset,
					in->size - in->offset);
			break ;
		}
		smol_compress_block(header, in, out);
		in->offset += block_size;
	}
}

static void	smol_compress_init(t_sml_buffer *in, t_sml_buffer *out)
{
	t_sml_header	header;

	memset(&header, 0, sizeof(header));
	out->offset = sizeof(t_sml_header);
	memcpy(&header.magic, SMOL_MAGIC, sizeof(header.magic));
	if (!smol_header_init(&header, in))
	{
		smol_write(out, &header, sizeof(t_sml_header));
		header.remaining = in->size;
		smol_write(out, in->data, in->size);
		return ;
	}
	smol_compress_blocks(&header, in, out);
	(void)smol_write_direct(out->data, 0, &header, sizeof(t_sml_header));
}

size_t	smol_compress(char *in_buf, size_t in_len, char *out_buf)
{
	t_sml_buffer	input_buffer;
	t_sml_buffer	output_buffer;

	memset(&input_buffer, 0, sizeof(t_sml_buffer));
	memset(&output_buffer, 0, sizeof(t_sml_buffer));
	input_buffer.data = in_buf;
	input_buffer.size = in_len;
	output_buffer.data = out_buf;
	smol_compress_init(&input_buffer, &output_buffer);
	return (output_buffer.offset);
}

static size_t	smol_decompress0(t_sml_buffer *in, t_sml_buffer *out)
{
	(void)in;
	(void)out;
	return (0);
}

size_t	smol_decompress(char *in_buf, size_t in_len, char *out_buf)
{
	t_sml_buffer	input_buffer;
	t_sml_buffer	output_buffer;

	memset(&input_buffer, 0, sizeof(t_sml_buffer));
	memset(&output_buffer, 0, sizeof(t_sml_buffer));
	input_buffer.data = in_buf;
	input_buffer.size = in_len;
	output_buffer.data = out_buf;
	smol_decompress0(&input_buffer, &output_buffer);
	return (output_buffer.offset);
}

// --------------------------------------- TESTS --- //

#define COMPRESS_SIZE_ERROR	"Compressed size is %zu, expected %zu\n"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void	print_hdr(t_sml_header *hdr)
{
	printf(">>> SMOL Header\n");
	// printf("magic: %s\n", hdr->magic);
	// printf("flags: %d\n", hdr->flags);
	printf("version: %d\n", hdr->flags & SMOL_VERSION_MASK);
	printf("metadata: %d\n", hdr->flags & SMOL_METADATA_MASK);
	printf("nblocks: %d\n", hdr->nblocks);
	printf("block_size: %d\n", hdr->block_size);
	printf("remaining: %d\n", hdr->remaining);
}

void	try(char *buf, size_t len)
{
	char	*compressed;
	size_t	compressed_len;
	char	*decompressed;
	size_t	tmp;

	printf("Trying to compress %zu bytes\n", len);
	printf("Data: '%s'\n", buf);
	compressed_len = smol_compress(buf, len, NULL);
	if (compressed_len == (size_t)-1)
		return ;
	compressed = malloc(compressed_len);
	decompressed = malloc(len + 1);
	if (compressed && decompressed)
	{
		tmp = smol_compress(buf, len, compressed);
		if (tmp != compressed_len)
			printf(COMPRESS_SIZE_ERROR, tmp, compressed_len);
		else
		{
			printf("Got %zu bytes\n", tmp);
			print_hdr((t_sml_header *)compressed);
			int fd = open("test.bin", O_CREAT | O_WRONLY, 0644);
			(void)!write(fd, compressed, tmp);
			close(fd);
		}
		// else if (tmp != smol_decompress(compressed, tmp, decompressed))
		// 	printf("Decompressed size is %zu, expected %zu\n", tmp, len);
		// else if (memcmp(buf, decompressed, len))
		// 	printf("Decompressed data is different from original\n");
		// else
		// 	printf("Success\n");
	}
	free(compressed);
	free(decompressed);
}

void	try_str(char *str)
{
	try(str, strlen(str));
}

int	main(void)
{
	// try_str("Hello, World!");
	try_str("aaaabbbbccccaaaabbbbccccaaaacccc");
	return (0);
}
