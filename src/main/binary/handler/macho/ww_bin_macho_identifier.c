/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_macho_identifier.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:00:04 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 12:07:21 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/macho.h>

t_ww_error	ww_bin_macho_identifier(t_ww_binary *bin, bool *identified)
{
	const size_t		read_size = sizeof(t_ww_macho_header);
	t_ww_macho_header	tmp;

	ft_memset(&tmp, 0, read_size);
	if ((size_t)read(bin->input_fd, &tmp, read_size) != read_size)
		return (ww_err_fmt(ERROR_IO, "Invalid Mach-O file (read failed)\n"));
	if (tmp.magic != MACHO_MAGIC_32 && tmp.magic != MACHO_MAGIC_64)
		return (ww_err_fmt(ERROR_IO, "Invalid Mach-O header "
				"(magic mismatch)\n"));
	ww_trace("Identified Mach-O header\n");
	*identified = true;
	return (ww_err_fmt(ERROR_INTERNAL, "'Mach-O' executable files are not "
			"supported yet"));
}
