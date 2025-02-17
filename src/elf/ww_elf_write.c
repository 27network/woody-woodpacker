/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_elf_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 16:35:34 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 20:37:47 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/io.h>
#include <ft/mem.h>
#include <sys/mman.h>
#include <ww/cli.h>
#include <ww/elf/x86.h>
#include <ww/elf/x86_64.h>
#include <ww/util.h>
#include <unistd.h>

bool	ww_elf_write(t_ww_args *args, t_ww_elf *elf, const char *path)
{
	const int	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	bool		err;

	if (fd == -1)
		return (false);
	err = true;
	if (elf->e32->e_ident[EI_CLASS] == ELFCLASS32)
		err = ww_elf_write_x86(args, fd, elf->e32, elf->size);
	else if (elf->e32->e_ident[EI_CLASS] == ELFCLASS64)
		err = ww_elf_write_x86_64(args, fd, elf->e64, elf->size);
	else
		ww_error("unknown ELF class, aborting writing\n");
	close(fd);
	return (!err);
}

// vim: ft=c
