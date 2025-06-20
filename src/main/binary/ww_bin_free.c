/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:41:10 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 20:47:41 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/mman.h>
#include <ww/binary.h>
#include <unistd.h>

void	ww_bin_free(t_ww_binary *bin)
{
	ww_trace("Freeing binary '%s'\n", bin->input);
	ww_debug("ww_bin_free(handler=%p)\n", (void *)bin->handler);
	if (bin->handler && bin->handler->free)
		bin->handler->free(bin->handler, bin);
	if (bin->blob)
		munmap(bin->blob, bin->blob_size);
	if (bin->input_fd > 0)
		close(bin->input_fd);
	if (bin->handler)
		free(bin->handler);
	if (bin->args->encryption_key_alloc)
		free(bin->args->encryption_key);
}
