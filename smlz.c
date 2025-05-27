/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smlz.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:45:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/26 18:51:05 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

// #define SMLZ_MAX_LENGTH		65535		// The max lookahead check for flm
// #define SMLZ_MIN_LENGTH		3			// The min match length for flm
// #define SMLZ_WINDOW_SIZE	4096		// The window size for flm

#define SMLZ_MAGIC			"SMLZ"
#define SMLZ_VERSION_MASK	0b00001111
#define SMLZ_METADATA_MASK	0b01110000

#define SMLZ_FLAG_V1		0b00000001

typedef struct s_smlz_header
{
	uint8_t		magic[4];
	uint16_t	nblocks;
	uint16_t	block_size;
	uint16_t	remaining;
	uint8_t		flags;
	uint8_t		reserved[1];
}	t_smlz_header;

typedef struct s_smlz_buffer
{
	char	*data;
	size_t	size;
	size_t	offset;
}	t_smlz_buffer;

size_t	smlz_compress(char *in_buf, size_t in_len, char *out_buf);
size_t	smlz_decompress(char *in_buf, size_t in_len, char *out_buf);

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

#ifndef PRINT
# define PRINT 0
#endif
#if PRINT
#define D(msg, ...) fprintf(stderr, "DEBUG: %s: " msg "\n", __func__ __VA_OPT__(,) ##__VA_ARGS__)
#else
#define D(msg, ...)
#endif

#define WINDOW_SIZE 65536     // Size of the sliding window
#define LOOKAHEAD_SIZE 65536	// Size of the lookahead buffer
#define MIN_MATCH_LENGTH 3		// Minimum match length to encode

static void find_longest_match2(
    const uint8_t *data,
    size_t data_len,
    size_t current_pos,
    uint16_t *offset,
    uint16_t *length
) {
    *offset = 0;
    *length = 0;

    // If we don't have enough data for a minimum match, don't bother searching
    if (current_pos + MIN_MATCH_LENGTH > data_len) {
        return;
    }

    // Calculate the search boundaries
    size_t window_start = (current_pos > WINDOW_SIZE) ? (current_pos - WINDOW_SIZE) : 0;
    size_t max_look_ahead = (current_pos + LOOKAHEAD_SIZE < data_len) ? 
                            LOOKAHEAD_SIZE : (data_len - current_pos);

    // Search for the longest match in the window
    for (size_t i = window_start; i < current_pos; i++) {
		if (current_pos - i >= 65536)
			continue ;
        // Determine match length
        size_t j = 0;
        while (j < max_look_ahead && 
               data[current_pos + j] == data[i + j]) {
            j++;
        }

        // Update if we found a longer match
        if (j >= MIN_MATCH_LENGTH && j > *length) {
            *offset = (uint16_t)(current_pos - i);
            *length = (j > 65535) ? 65535 : (uint16_t)j;
        }
    }
}

[[nodiscard]]
static size_t	smlz_write_direct(char *buf, size_t offset, void *data,
					size_t len)
{
	if (buf)
		memcpy(buf + offset, data, len);
	return (len);
}

static void hexdump(void *buf, size_t len)
{
	char tmp[3];
	tmp[2] = '\0';
	for (size_t i = 0; i < len; i++)
	{
		tmp[0] = "0123456789abcdef"[*(((uint8_t*)buf) + i) >> 4];
		tmp[1] = "0123456789abcdef"[*(((uint8_t*)buf) + i) & 0xf];
		printf("%s ", tmp);
	}
	printf("\n");
}

static size_t	smlz_write(t_smlz_buffer *buf, void *data, size_t len)
{
	const size_t	ret_len
		= smlz_write_direct(buf->data, buf->offset, data, len);

#if PRINT
	if (buf->data)
	{
		D("written:");
		hexdump(buf->data + buf->offset, len);
	}
#endif
	buf->offset += ret_len;
	return (ret_len);
}

static bool	smlz_header_init(t_smlz_header *header, t_smlz_buffer *in)
{
	size_t	size;

	size = in->size;
	header->flags = SMLZ_FLAG_V1;
	header->block_size = 8;
	while (size >= 64 && header->block_size < 32768)
	{
		header->block_size *= 2;
		size >>= 2;
	}
	return (true);
}

#define SMLZ_BITS 8  // I hope thats not changing soon :pray:

typedef __attribute__((packed)) struct s_smlz_token
{
	uint16_t	offset;
	uint16_t	length;
}	t_smlz_token;

// Return true si le contenu à (in->data + in->offset) peut être compressé,
// et écrit le résultat compressé dans (out->data + out->offset)
static inline bool	smlz_compress_litteral(t_smlz_buffer *in,
					t_smlz_buffer *out)
{
	t_smlz_token	token;

//	token.offset = find_longest_match((const uint8_t *)in->data, \
//											in->offset, in->size, &token.length);
	find_longest_match2(in->data, in->size, in->offset, &token.offset, &token.length);
	D("tkn: {off=%u,len=%u}", token.offset, token.length);
	if (!token.offset || token.length <= sizeof(token))
		return (false);
	smlz_write(out, &token, sizeof(t_smlz_token));
	in->offset += token.length;
	return (true);
}

static size_t	smlz_compress_block(t_smlz_header *header, t_smlz_buffer *in,
					t_smlz_buffer *out)
{
	const size_t	block_size = header->block_size;
	char			*block_header;
	size_t			written;

	// Set the block header 
	block_header = out->data + out->offset;
	out->offset += block_size / SMLZ_BITS;

	// Write out compressed/litterals sequentially until we
	// get enough to fill the block
	written = 0;
	D("writing block");
	while (true)
	{
#if PRINT
		write(1, in->data + in->offset, in->size - in->offset);
		write(1, "\n", 1);
#endif
		if (written >= block_size)
		{
			D("written enough for block_size, exiting");
			break ;
		}
		if (in->offset >= in->size)
		{
			D("no more data to write, exiting");
			break ;
		}

		D("writing byte %zu/%zu", written + 1, block_size);
		if (smlz_compress_litteral(in, out))
		{
			D("was compressed, setting header bit");
			if (out->data)
			{
				D("bit=%zu, bs=%zu, w=%zu", written / SMLZ_BITS, block_size, written);
				block_header[written / SMLZ_BITS]
					|= (1 << ((SMLZ_BITS - (written + 1)) % SMLZ_BITS));
			}
		}
		else
		{
	//		D("wasn't compressable, writing litteral (%#x)", in->data[in->offset]);
			// Couldn't compress litteral, writing the character as is
			in->offset += smlz_write(out, in->data + in->offset, 1);
		}
		written++;
	}
	D("wrote %zu bytes", written);
	return (written);
}

static void	smlz_compress_blocks(t_smlz_header *header, t_smlz_buffer *in,
					t_smlz_buffer *out)
{
	size_t	last_block_size;

	last_block_size = 0;
	header->remaining = 0;
	while (in->offset < in->size)
	{
		last_block_size = smlz_compress_block(header, in, out);
		header->nblocks++;
	}
	if (last_block_size != header->block_size)
		header->remaining = last_block_size;
}

static void	smlz_compress_init(t_smlz_buffer *in, t_smlz_buffer *out)
{
	t_smlz_header	header;

	memset(&header, 0, sizeof(header));
	out->offset = sizeof(t_smlz_header);
	memcpy(&header.magic, SMLZ_MAGIC, sizeof(header.magic));
	if (!smlz_header_init(&header, in))
	{
		smlz_write(out, &header, sizeof(t_smlz_header));
		header.remaining = in->size;
		smlz_write(out, in->data, in->size);
		return ;
	}
	smlz_compress_blocks(&header, in, out);
	(void)smlz_write_direct(out->data, 0, &header, sizeof(t_smlz_header));
}

size_t	smlz_compress(char *in_buf, size_t in_len, char *out_buf)
{
	t_smlz_buffer	input_buffer;
	t_smlz_buffer	output_buffer;

	memset(&input_buffer, 0, sizeof(t_smlz_buffer));
	memset(&output_buffer, 0, sizeof(t_smlz_buffer));
	input_buffer.data = in_buf;
	input_buffer.size = in_len;
	output_buffer.data = out_buf;
	smlz_compress_init(&input_buffer, &output_buffer);
	return (output_buffer.offset);
}

// --------------------------------------- TESTS --- //

#define COMPRESS_SIZE_ERROR	"Compressed size is %zu, expected %zu\n"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void	print_hdr(t_smlz_header *hdr)
{
	printf(">>> SMLZ Header\n");
	printf("magic: %s\n", hdr->magic);
	// printf("flags: %d\n", hdr->flags);
	printf("version: %d\n", hdr->flags & SMLZ_VERSION_MASK);
	printf("metadata: %d\n", hdr->flags & SMLZ_METADATA_MASK);
	printf("nblocks: %d\n", hdr->nblocks);
	printf("block_size: %d\n", hdr->block_size);
	printf("remaining: %d\n", hdr->remaining);
}

void	try(char *buf, size_t len)
{
	char	*compressed;
	size_t	compressed_len;
	size_t	tmp;

	printf("Trying to compress %zu bytes\n", len);
	printf("Data: '%s'\n", buf);
	printf("Compressing once to get length:");
	compressed_len = smlz_compress(buf, len, NULL);
	if (compressed_len == (size_t)-1)
		return ;
	printf("%zu\n", compressed_len);
	compressed = calloc(compressed_len, 1);
	if (compressed)
	{
		printf("compressing twice for real\n");
		tmp = smlz_compress(buf, len, compressed);
		if (tmp != compressed_len)
			printf(COMPRESS_SIZE_ERROR, tmp, compressed_len);
		else
		{
			printf("Got %zu bytes\n", tmp);
			print_hdr((t_smlz_header *)compressed);
			int fd = open("test.bin", O_CREAT | O_WRONLY, 0644);
			(void)!write(fd, compressed, tmp);
			close(fd);
		}
	}
	free(compressed);
}

void	try_file(int fd, off_t file_size)
{
	void	*buffer;

	if (file_size == 0) {
		printf("special case 0 size moment\n");
		try("", 0);
		return;
	}
	buffer = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buffer == MAP_FAILED) {
		perror("mmap");
		return ;
	}
	try(buffer, file_size);
	munmap(buffer, file_size);
}

int	main(int argc, char **argv)
{
	int		fd;
	off_t	file_size;

	// printf("%ld\n", sizeof(t_smlz_header));
	// try_str("Hello, World!");
	if (argc != 2)
	{
		fprintf(stderr, "Usage: ./smlz file_to_compress");
		return 1;
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return 1;
	}
	file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	try_file(fd, file_size);
	return (0);
}
