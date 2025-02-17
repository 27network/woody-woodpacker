/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_handler_register.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:54:20 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 22:13:08 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>
#include <stdlib.h>

static void	ww_bin_handler_check(t_ww_binary_handler handler)
{
	if (!handler.type)
	{
		ww_error("binary handler has no defined type\n");
		exit(-1);
	}
	if (!handler.identify)
	{
		ww_error("binary handler '%s' has no 'identify' function\n",
			handler.type);
		exit(-1);
	}
	if (!handler.read)
	{
		ww_error("binary handler '%s' has no 'read' function\n",
			handler.type);
		exit(-1);
	}
	if (!handler.write)
	{
		ww_error("binary handler '%s' has no 'write' function\n",
			handler.type);
		exit(-1);
	}
}

void	ww_bin_handler_register(t_ww_binary_handler handler)
{
	t_ww_binary_handler	*registry;
	size_t				i;

	ww_bin_handler_check(handler);
	i = 0;
	registry = ww_bin_handler_registry();
	while (i < WW_BIN_HANDLERS_MAX)
	{
		if (!registry[i].type)
		{
			registry[i] = handler;
			return ;
		}
		i++;
	}
	ww_error("ww_bin_handler_register: too many handlers registered\n");
	exit(-1);
}
