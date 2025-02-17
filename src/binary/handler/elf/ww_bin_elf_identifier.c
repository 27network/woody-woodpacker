/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_identifier.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:51:46 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:14:29 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/elf.h>
#include <ww/log.h>

enum e_ww_error	ww_bin_elf_identifier(t_ww_binary *bin)
{
	Elf32_Ehdr		tmp;
	const size_t	size = sizeof(Elf32_Ehdr);

	ft_memset(&tmp, 0, sizeof(Elf32_Ehdr));
	if (read(bin->input_fd, &tmp, size) != size)
		return (WW_ERROR);
	if (ft_memcmp(&tmp.e_ident, ELFMAG, SELFMAG) != 0)
		return (WW_ERROR);
	return (WW_OK);
}
