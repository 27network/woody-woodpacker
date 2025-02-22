/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:36:38 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:16:49 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>

enum e_ww_error	ww_bin_read(t_ww_binary *bin)
{
	if (bin && bin->handler && bin->handler->read)
	{
		ww_bin_map(bin);
		ww_trace("Reading %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->read(bin->handler, bin));
	}
	ww_trace("No 'read' handler for %s\n", bin->input);
	return (WW_ERROR);
}
