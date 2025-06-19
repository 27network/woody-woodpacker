/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:30:37 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/19 17:31:16 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <elfstream.h>

static inline bool	elfstream_validate(t_elfstream *self, int fd)
{
	ft_memset(self, 0, sizeof(t_elfstream));
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
		if (read(fd, &self->ehdr64, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr))
			return (false);
		self->segment_count = self->ehdr64.e_phnum;
		self->section_count = self->ehdr64.e_shnum;
		lseek(fd, self->ehdr64.e_phoff, SEEK_SET);
	}
	return (true);
}

static inline size_t	p(t_elfstream *self, size_t off_32, size_t off_64)
{
	if (self->bitness == ELFSTREAM_32)
		return (off_32);
	return (off_64);
}

static inline bool	elfstream_segments_fill(t_elfstream *self, int fd,
						size_t off)
{
	size_t			i;
	const ssize_t	to_read = p(self, sizeof(Elf32_Phdr), sizeof(Elf64_Phdr));
	t_elf_segment	*segment;

	i = 0;
	lseek(fd, off, SEEK_SET);
	while (i < self->segment_count)
	{
		segment = self->segments + i;
		segment->stream = self;
		if (read(fd, &segment->phdr32, to_read) != to_read)
			return (false);
		DBG("j'ai un segment %zu", i);
		DBG("offset: %#lx", segment->phdr64.p_offset);
		DBG("filesz: %#lx", segment->phdr64.p_filesz);
		segment->content = elfstream_source_fd(self, fd,
				p(self, segment->phdr32.p_offset, segment->phdr64.p_offset),
				p(self, segment->phdr32.p_filesz, segment->phdr64.p_filesz));
		i++;
	}
	return (true);
}

[[maybe_unused]]
static inline bool	elfstream_sections_fill(t_elfstream *self, int fd)
{
	const ssize_t	to_read = p(self, sizeof(Elf32_Shdr), sizeof(Elf64_Shdr));
	const size_t	off = p(self, self->ehdr32.e_shoff, self->ehdr64.e_shoff);
	t_elf_section	*section;
	size_t			i;

	i = 0;
	DBG("starting at %#lx (should read %#lx)", off, to_read);
	lseek(fd, off, SEEK_SET);
	while (i < self->section_count)
	{
		section = self->sections + i;
		DBG("reading section %zu", i);
		section->stream = self;
		if (read(fd, &section->shdr32, to_read) != to_read)
			return (false);
		DBG("section off=%#lx, size=%#lx", section->shdr64.sh_offset,
			section->shdr64.sh_size);
		section->content = elfstream_source_fd(self, fd,
				p(self, section->shdr32.sh_offset, section->shdr64.sh_offset),
				p(self, section->shdr32.sh_size, section->shdr64.sh_size));
		i++;
	}
	return (true);
}

enum e_elfstream_error	elfstream_open(t_elfstream *self, int fd)
{
	size_t		offset;

	if (!elfstream_validate(self, fd))
		return (ELFSTREAM_INVALID);
	self->segments = ft_calloc(self->segment_count, sizeof(t_elf_segment));
	self->sections = ft_calloc(self->section_count, sizeof(t_elf_section));
	if (!self->segments || !self->sections)
	{
		elfstream_close(self);
		return (ELFSTREAM_ALLOC);
	}
	offset = p(self, self->ehdr32.e_phoff, self->ehdr64.e_phoff);
	if (!elfstream_segments_fill(self, fd, offset))
	{
		elfstream_close(self);
		return (ELFSTREAM_ERROR);
	}
	if (!elfstream_sections_fill(self, fd))
	{
		elfstream_close(self);
		return (ELFSTREAM_ERROR);
	}
	return (ELFSTREAM_OK);
}
