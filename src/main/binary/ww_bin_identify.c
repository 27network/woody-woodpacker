/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_identify.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:51:07 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:17:49 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <sys/types.h>
#include <unistd.h>
#include <ww/binary.h>

enum e_ww_error	ww_bin_identify(t_ww_binary *bin)
{
	t_ww_binary_handler	*registry;
	size_t				i;

	registry = ww_bin_handler_registry();
	i = 0;
	while (i < WW_BIN_HANDLERS_MAX && registry[i].type)
	{
		ww_trace("Trying to identify as '%s'\n", registry[i].type);
		if (registry[i].identify(bin) == WW_OK)
		{
			ww_info("Identified %s as '%s'\n", bin->input, registry[i].type);
			ww_trace("Allocating %d bytes for '%s' handler\n",
				(int)registry[i].struct_size, registry[i].type);
			bin->handler = ft_calloc(1, registry[i].struct_size);
			if (bin->handler)
				*(bin->handler) = registry[i];
			else
				return (WW_ERROR);
			return (WW_OK);
		}
		lseek(bin->input_fd, 0, SEEK_SET);
		i++;
	}
	ww_trace("Couldn't identify as any known binary type\n");
	return (WW_ERROR);
}
