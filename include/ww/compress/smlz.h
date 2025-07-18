/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smlz.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 18:54:45 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/18 10:05:36 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SMLZ_H
# define SMLZ_H

# include <stddef.h>
# include <stdint.h>

size_t	smlz_compress(char *in_buf, size_t in_len, char *out_buf);
size_t	smlz_decompress(char *in_buf, size_t in_len, char *out_buf);

# ifdef _SMLZ_IMPL

#  define SMLZ_MAGIC			"SMLZ"
#  define SMLZ_VERSION_MASK		0b00001111
#  define SMLZ_METADATA_MASK	0b01110000

#  define SMLZ_FLAG_V1			0b00000001

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

#  define SMLZ_BITS 8  // I hope thats not changing soon :pray:

typedef struct s_smlz_token
{
	uint16_t	offset;
	uint16_t	length;
}	t_smlz_token;

void	smlz_compress_impl(t_smlz_buffer *in, t_smlz_buffer *out,
					size_t block_size_override);

void	smlz_compress_blocks(t_smlz_header *header, t_smlz_buffer *in,
					t_smlz_buffer *out);
size_t	smlz_compress_block(t_smlz_header *header, t_smlz_buffer *in,
					t_smlz_buffer *out);
bool	smlz_compress_litteral(t_smlz_buffer *in, t_smlz_buffer *out);

bool	smlz_header_init(t_smlz_header *header, t_smlz_buffer *in);

size_t	smlz_write(t_smlz_buffer *buf, void *data, size_t len);
size_t	smlz_write_direct(char *buf, size_t offset, void *data, size_t len);

void	smlz_find_longest_match(const unsigned char *data, size_t data_len,
					size_t current_pos, uint16_t *offset, uint16_t *length);

# endif // _SMLZ_IMPL

#endif // SMLZ_H
