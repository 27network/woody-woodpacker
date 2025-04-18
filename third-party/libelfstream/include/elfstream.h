/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:29:50 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/17 11:49:39 by kiroussa         ###   ########.fr       */
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

const char
*elfstream_strerror(enum e_elfstream_error error);

enum e_elfstream_bitness
{
	ELFSTREAM_32 = 32,
	ELFSTREAM_64 = 64,
};

typedef struct s_elfstream	t_elfstream;

enum e_content_source_type
{
	CONTENT_SOURCE_FILE = 0,
	CONTENT_SOURCE_MEMORY,
};

typedef struct s_content_source
{
	enum e_content_source_type	type;
	union
	{
		struct
		{
			int			fd;
			size_t		offset;
			size_t		size;
		}	s_file;
		struct
		{
			const char	*data;
			size_t		size;
		}	s_memory;
	};
	struct s_content_source		*next;
}	t_content_source;

t_content_source
*elfstream_source_fd(int fd, size_t offset, size_t size);

t_content_source
*elfstream_source_data(const char *data, size_t size);

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

typedef struct s_elf_section
{
	t_elfstream					*stream;
	union
	{
		Elf32_Shdr	shdr32;
		Elf64_Shdr	shdr64;
	};
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
