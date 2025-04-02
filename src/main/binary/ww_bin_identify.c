/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_identify.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:51:07 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/02 17:11:51 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <sys/types.h>
#include <unistd.h>
#include <ww/binary.h>

static t_ww_error	ww_bin_handle_identifier(t_ww_binary *bin,
							t_ww_error err, t_ww_binary_handler *registry)
{
	if (err.type)
		return (err);
	ww_info("Identified %s as '%s'\n", bin->input, registry[i].type);
	ww_trace("Allocating %d bytes for '%s' handler\n",
		(int)registry[i].struct_size, registry[i].type);
	bin->handler = ft_calloc(1, registry[i].struct_size);
	if (!bin->handler)
		return (ww_err_str(ERROR_ALLOC, "binary handler"));
	*(bin->handler) = registry[i];
	return (ww_ok());
}

t_ww_error	ww_bin_identify(t_ww_binary *bin)
{
	t_ww_binary_handler	*registry;
	size_t				i;
	bool				found;
	t_ww_error			err;

	registry = ww_bin_handler_registry();
	i = 0;
	while (i < WW_BIN_HANDLERS_MAX && registry[i].type)
	{
		ww_trace("Trying to identify as '%s'\n", registry[i].type);
		found = false;
		lseek(bin->input_fd, 0, SEEK_SET);
		err = registry[i].identify(bin, &found);
		if (found)
			return (ww_bin_handle_identifier(bin, err, &registry[i]));
		i++;
	}
	return (ww_err_fmt(ERROR_IO,
			"Couldn't identify '%s' as any known binary type\n", bin->input));
}
