/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:37:16 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:16:40 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>

enum e_ww_error	ww_bin_process(t_ww_binary *bin)
{
	if (bin && bin->handler && bin->handler->process)
	{
		ww_trace("Processing %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->process(bin->handler, bin));
	}
	ww_trace("No 'process' handler for %s\n", bin->input);
	return (WW_ERROR);
}
