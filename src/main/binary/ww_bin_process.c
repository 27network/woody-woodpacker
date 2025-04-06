/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:37:16 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 09:38:12 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>

t_ww_error	ww_bin_process(t_ww_binary *bin)
{
	if (bin && bin->handler && bin->handler->process)
	{
		ww_trace("Processing %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->process(bin->handler, bin));
	}
	return (ww_err_fmt(ERROR_INTERNAL, "No 'process' handler for %s\n",
			bin->input));
}
