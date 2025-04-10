/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:30:37 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 20:29:51 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <elfstream.h>

static inline bool	elfstream_validate(t_elfstream *self, int fd)
{
	lseek(fd, 0, SEEK_SET);
	self->fd = fd;
	if (read(fd, &self->ehdr32, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)
		|| ft_memcmp(&self->ehdr32.e_ident[0], ELFMAG, SELFMAG)
		|| self->ehdr32.e_ident[EI_CLASS] == 0)
		return (false);
	self->bitness = ELFSTREAM_64;
	if (self->ehdr32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		self->bitness = ELFSTREAM_32;
		self->segment_count = self->ehdr32.e_phnum;
		self->section_count = self->ehdr32.e_shnum;
		lseek(fd, self->ehdr32.e_phoff, SEEK_SET);
	}
	else
	{
		lseek(fd, 0, SEEK_SET);
		if (read(fd, &self->ehdr64, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdh))
			return (false);
		self->segment_count = self->ehdr64.e_phnum;
		self->section_count = self->ehdr64.e_shnum;
		lseek(fd, self->ehdr64.e_phoff, SEEK_SET);
	}
	return (true);
}

static inline bool	elfstream_segments_fill(t_elfstream *self, int fd)
{
	size_t	i;
	size_t	to_read;

	to_read = self->ehdr32.e_phentsize;
	if (self->bitness == ELFSTREAM_64)
		to_read = self->ehdr64.e_phentsize;
	i = 0;
	while (i < self->segment_count)
	{
		self->segments[i].stream = self;
		if (read(fd, &self->segments[i].phdr64, to_read) != to_read)
			return (false);
		self->segments[i].content = elfstream_source_fd(
			
		);
		i++;
	}
}

enum e_elfstream_error	elfstream_open(t_elfstream *self, int fd)
{
	if (!elfstream_validate(self, fd))
		return (ELFSTREAM_INVALID);
	self->segments = ft_calloc(self->segment_count, sizeof(t_elf_segment));
	if (!self->segments)
		return (ELFSTREAM_ALLOC);
	if (!elfstream_segments_fill(self, fd))
	{
		elfstream_close(self);
		*self = NULL;
		return (ELFSTREAM_ERROR);
	}
	return (ELFSTREAM_OK);
}
