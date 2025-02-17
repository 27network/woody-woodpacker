/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:28:40 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:16:26 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <ww/binary.h>
#include <unistd.h>

enum e_ww_error	ww_bin_map(t_ww_binary *bin)
{
	off_t	current_pos;

	ww_trace("Mapping %s\n", bin->input);
	current_pos = lseek(bin->input_fd, (off_t)0, SEEK_SET);
	if (current_pos == -1)
		ww_error("ww_bin_map: lseek failed: %m\n");
	if (current_pos == -1)
		return (WW_ERROR);
	bin->blob_size = (size_t)lseek(bin->input_fd, 0, SEEK_END);
	if (bin->blob_size == (size_t)-1)
		ww_error("ww_bin_map: lseek failed: %m\n");
	if (bin->blob_size == (size_t)-1)
		return (WW_ERROR);
	lseek(bin->input_fd, 0, SEEK_SET);
	bin->blob = mmap(NULL, bin->blob_size, PROT_READ, MAP_PRIVATE,
			bin->input_fd, 0);
	ww_trace("Mapped %s to %p\n", bin->input, (void *)bin->blob);
	if (bin->blob == MAP_FAILED)
		ww_error("ww_bin_map: mmap failed: %m\n");
	else
		return (WW_OK);
	return (WW_ERROR);
}
