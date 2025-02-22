/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_elf_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:28:43 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 17:32:59 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_LOG_INTERNAL__
#include <ww/log.h>

#include <fcntl.h>
#include <ft/io.h>
#include <ft/mem.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <ww/elf.h>

static bool	ww_elf_size(t_ww_elf *elf)
{
	off_t	current_pos;

	current_pos = lseek(elf->fd, (off_t)0, SEEK_SET);
	if (current_pos == -1)
		return (false);
	elf->size = (size_t)lseek(elf->fd, 0, SEEK_END);
	if (elf->size == (size_t)-1)
		return (false);
	lseek(elf->fd, 0, SEEK_SET);
	return (true);
}

__attribute__((format(printf, 1, 2)))
static bool	ww_elf_error(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ww_vlog(LOG_ERROR, fmt, args);
	va_end(args);
	return (false);
}

static bool	ww_elf_validate(t_ww_elf *elf)
{
	if (elf->size <= 4)
		return (ww_elf_error("invalid ELF file size (too small)\n"));
	if (elf->size > SIZE_MAX)
		return (ww_elf_error("invalid ELF file size (too big)\n"));
	if (ft_memcmp(elf->e32->e_ident, ELFMAG, SELFMAG) != 0)
		return (ww_elf_error("invalid ELF magic number\n"));
	if (elf->e32->e_ident[EI_CLASS] == ELFCLASS32 && !WW_SUPPORT_32)
		return (ww_elf_error("invalid ELF class: 32-bit is unsupported\n"));
	if (elf->e32->e_machine != EM_X86_64
		&& elf->e32->e_machine != EM_386)
		return (ww_elf_error("unsupported ELF machine type "
				"(need x86_64 or i386)\n"));
	return (true);
}

static t_ww_elf	*ww_elf_map(t_ww_elf *elf)
{
	elf->e32 = mmap(NULL, elf->size, PROT_READ, MAP_PRIVATE, elf->fd, 0);
	if (elf->e32 == MAP_FAILED)
	{
		ww_error("ww_elf_map: mmap failed: %m\n");
		elf->e32 = NULL;
		ww_elf_free(&elf);
		return (NULL);
	}
	if (!ww_elf_validate(elf))
	{
		ww_elf_free(&elf);
		return (NULL);
	}
	return (elf);
}

t_ww_elf	*ww_elf_parse(const char *path)
{
	const int	fd = open(path, O_RDONLY);
	t_ww_elf	*elf;

	if (fd == -1)
		ww_error("ww_elf_parse: open failed: %m\n");
	if (fd == -1)
		return (NULL);
	elf = ft_calloc(1, sizeof(t_ww_elf));
	if (!elf)
	{
		ww_error("ww_elf_parse: allocation error\n");
		ft_closep((int *) &fd);
		return (NULL);
	}
	elf->fd = fd;
	if (!ww_elf_size(elf))
	{
		ww_error("error while reading ELF file: %m\n");
		ww_elf_free(&elf);
		return (NULL);
	}
	return (ww_elf_map(elf));
}
