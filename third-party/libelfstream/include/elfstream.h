/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:29:50 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/12 16:50:45 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELFSTREAM_H
# define ELFSTREAM_H

# include <elf.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

# if ELFSTREAM_DEBUG
#  include <stdio.h>
#  define DBG(msg, ...) fprintf(stderr, "DEBUG: %s: " msg "\n", __func__ __VA_OPT__(,) ##__VA_ARGS__)
# else
#  define DBG(msg, ...)
# endif

/**
 *
 */
enum e_elfstream_error
{
	ELFSTREAM_OK = 0,
	ELFSTREAM_ERROR,
	ELFSTREAM_INVALID,
	ELFSTREAM_IO,
	ELFSTREAM_EOF,
	ELFSTREAM_ALLOC,
	_ELFSTREAM_ERROR_MAX,
};

/**
 *
 */
const char
*elfstream_strerror(enum e_elfstream_error error);

/**
 *
 */
enum e_elfstream_bitness
{
	ELFSTREAM_32 = 32,
	ELFSTREAM_64 = 64,
};

typedef struct s_elfstream	t_elfstream;

/**
 *
 */
enum e_content_source_type
{
	CONTENT_SOURCE_UNKNOWN = 0,
	CONTENT_SOURCE_FILE,
	CONTENT_SOURCE_MEMORY,
};

/**
 *
 */
typedef struct s_content_source
{
	enum e_content_source_type	type;
	union
	{
		struct
		{
			int			fd;
			size_t		offset;
		}	s_file;
		struct
		{
			const char	*data;
		}	s_memory;
	};
	size_t						size;
	enum e_elfstream_error		(*write_fn)(struct s_content_source *self,
			int fd);
	struct s_content_source		*next;
}	t_content_source;

/**
 *
 */
size_t
elfstream_content_size(t_content_source *content);

/**
 *
 */
void
elfstream_content_free(t_content_source *content);

/**
 *
 */
t_content_source
*elfstream_source_fd(t_elfstream *stream, int fd, size_t offset, size_t size);

/**
 *
 */
enum e_elfstream_error
elfstream_write_source_fd(t_content_source *self, int fd);

/**
 *
 */
t_content_source
*elfstream_source_data(const char *data, size_t size);

/**
 *
 */
enum e_elfstream_error
elfstream_write_source_data(t_content_source *self, int fd);

/**
 *
 */
typedef struct s_elf_segment
{
	t_elfstream					*stream;
	union
	{
		Elf32_Phdr	phdr32;
		Elf64_Phdr	phdr64;
	};
	t_content_source			*content;
}	t_elf_segment;

/**
 *
 */
typedef struct s_elf_section
{
	t_elfstream					*stream;
	union
	{
		Elf32_Shdr	shdr32;
		Elf64_Shdr	shdr64;
	};
	t_content_source			*content;
}	t_elf_section;

/**
 *
 */
typedef struct s_elfstream
{
	int							fd;
	enum e_elfstream_bitness	bitness;
	union
	{
		Elf32_Ehdr				ehdr32;
		Elf64_Ehdr				ehdr64;
	};
	size_t						segment_count;
	t_elf_segment				*segments;
	size_t						section_count;
	t_elf_section				*sections;
}	t_elfstream;

/**
 *
 */
enum e_elfstream_error
elfstream_open(t_elfstream *self, int fd);

/**
 *
 */
enum e_elfstream_error
elfstream_write(t_elfstream *self, int fd);

/**
 * 
 */
enum e_elfstream_error
elfstream_close(t_elfstream *self);

#endif // ELFSTREAM_H
