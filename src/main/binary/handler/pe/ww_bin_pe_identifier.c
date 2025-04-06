/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_pe_identifier.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:40:07 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 12:07:39 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary/pe.h>

t_ww_error	ww_bin_pe_identifier(t_ww_binary *bin, bool *identified)
{
	const size_t		read_size = sizeof(t_ww_pe_dos_header);
	t_ww_pe_dos_header	tmp;
	const size_t		read_size2 = sizeof(t_ww_pe_nt_header);
	t_ww_pe_nt_header	tmp2;

	ft_memset(&tmp, 0, sizeof(t_ww_pe_dos_header));
	if ((size_t)read(bin->input_fd, &tmp, read_size) != read_size)
		return (ww_err_fmt(ERROR_IO, "Invalid PE file (read failed)\n"));
	if (ft_memcmp(&tmp.e_magic, PE_MAGIC, 2) != 0)
		return (ww_err_fmt(ERROR_IO, "Invalid PE header (magic mismatch)\n"));
	ww_trace("Identified DOS header\n");
	if (lseek(bin->input_fd, tmp.e_lfanew, SEEK_SET) < 0)
		return (ww_err_fmt(ERROR_IO, "Invalid PE file (lseek failed)\n"));
	if ((size_t)read(bin->input_fd, &tmp2, read_size2) != read_size2)
		return (ww_err_fmt(ERROR_IO, "Invalid PE file (read failed)\n"));
	if (ft_memcmp(&tmp2.signature, "PE\0\0", 4) != 0)
		return (ww_err_fmt(ERROR_IO, "Invalid PE signature (0x%x)\n",
				tmp2.signature));
	ww_trace("Identified NT header\n");
	*identified = true;
	return (ww_err_fmt(ERROR_INTERNAL, "'PE' executable files are not "
			"supported yet"));
}
