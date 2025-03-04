/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smol_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:45:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/03/04 14:22:26 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SMOL_MAX_LENGTH		255
#define SMOL_MIN_LENGTH		3
#define SMOL_WINDOW_SIZE	256
// #define SMOL_BLOCK_VALUES	8
// #define SMOL_BLOCK_SIZE		8 * SMOL_BLOCK_VALUES

#define SMOL_MAGIC			"SM0L"
#define SMOL_FLAG_V1		0b00000001
#define SMOL_VERSION_MASK	0b00001111
#define SMOL_FLAG_TRAILING	0b10000000
#define SMOL_METADATA_MASK	0b01110000

typedef struct s_sml_header
{
	uint8_t		magic[4];
	uint8_t		flags;
	uint8_t		block_size;
	uint16_t	nblocks;
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
	uint8_t *length
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

static void	smol_compress0(t_sml_buffer *in, t_sml_buffer *out)
{
	t_sml_header	header;

	memset(&header, 0, sizeof(header));
	out->offset = sizeof(t_sml_header);
	memcpy(&header.magic, SMOL_MAGIC, sizeof(header.magic));
	header.flags = SMOL_FLAG_V1;
	while (in->offset < in->size)
	{
		printf("Remaining: %zu (block: %zu)\n", in->size - in->offset, \
			SMOL_BLOCK_SIZE);
		if (in->offset + SMOL_BLOCK_SIZE > in->size)
		{
			smol_block_write(out, in, true);
			header.flags |= SMOL_FLAG_TRAILING;
		}
		header.nblocks++;
	}
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
	smol_compress0(&input_buffer, &output_buffer);
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

void	try(char *buf, size_t len)
{
	char	*compressed;
	size_t	compressed_len;
	char	*decompressed;
	size_t	tmp;

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
		else if (tmp != smol_decompress(compressed, tmp, decompressed))
			printf("Decompressed size is %zu, expected %zu\n", tmp, len);
		else if (memcmp(buf, decompressed, len))
			printf("Decompressed data is different from original\n");
		else
			printf("Success\n");
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
	try_str("Hello, World!");
	return (0);
}
