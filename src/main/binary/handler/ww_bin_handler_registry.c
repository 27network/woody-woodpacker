/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_handler_registry.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:55:21 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 21:06:28 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <ww/binary.h>
#include <ww/log.h>

t_ww_binary_handler	*ww_bin_handler_registry(void)
{
	static t_ww_binary_handler	handlers[WW_BIN_HANDLERS_MAX];
	static bool					initialized = false;

	if (!initialized)
	{
		initialized = true;
		ft_memset(handlers, 0, sizeof(t_ww_binary_handler)
			* WW_BIN_HANDLERS_MAX);
	}
	return (handlers);
}

void	ww_bin_handler_registry_dump(void)
{
	t_ww_binary_handler	*registry;
	size_t				i;

	i = 0;
	registry = ww_bin_handler_registry();
	while (i < WW_BIN_HANDLERS_MAX && registry[i].type)
	{
		ww_trace("Found handler for type '%s'\n", registry[i].type);
		i++;
	}
}
