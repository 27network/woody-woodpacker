/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_open.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:30:37 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/11 20:16:09 by kiroussa         ###   ########.fr       */
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

static inline size_t	pick(t_elfstream *self, size_t off_32, size_t off_64)
{
	if (self->bitness == ELFSTREAM_32)
		return (off_32);
	return (off_64);
}

static inline bool	elfstream_segments_fill(t_elfstream *self, int fd, size_t off)
{
	size_t			i;
	const ssize_t	to_read = pick(self, sizeof(Elf32_Phdr),
			sizeof(Elf64_Phdr));
	const size_t	offset_offset = pick(self, offsetof(Elf32_Phdr, p_offset),
			offsetof(Elf64_Phdr, p_offset));
	const size_t	filesz_offset = pick(self, offsetof(Elf32_Phdr, p_filesz),
			offsetof(Elf64_Phdr, p_filesz));
	char			*data;

	i = 0;
	lseek(fd, off, SEEK_SET);
	while (i < self->segment_count)
	{
		self->segments[i].stream = self;
		if (read(fd, &self->segments[i].phdr32, to_read) != to_read)
			return (false);
		data = (char *) &self->segments[i].phdr32;
		printf("j'ai un segment %zu\n", i);
		printf("offset: %#lx\n", self->segments[i].phdr64.p_offset);
		printf("filesz: %#lx\n", self->segments[i].phdr64.p_filesz);
		self->segments[i].content = elfstream_source_fd(self, fd,
				*((Elf64_Off *)(data + offset_offset)),
				*((Elf64_Off *)(data + filesz_offset))); //TODO: fix this for 32bit
		i++;
	}
	return (true);
}

static inline bool	elfstream_sections_fill(t_elfstream *self, int fd)
{
	size_t			i;
	const ssize_t	to_read = pick(self, sizeof(Elf32_Shdr),
			sizeof(Elf64_Shdr));
	size_t			read_offset = pick(self, self->ehdr32.e_shoff,
			self->ehdr64.e_shoff);

	i = 0;
	DBG("starting at %#lx (should read %#lx)", read_offset, to_read);
	lseek(fd, read_offset, SEEK_SET);
	while (i < self->section_count)
	{
		DBG("reading section %zu", i);
		self->sections[i].stream = self;
		ssize_t nread = read(fd, &self->sections[i].shdr32, to_read);
		DBG("we have read %zu bytes", nread);
		if (nread != to_read)
			return (false);
		DBG("read section %zu (loc: %p)", i, &self->sections[i].shdr32);
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
	offset = pick(self, self->ehdr32.e_phoff, self->ehdr64.e_phoff);
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
