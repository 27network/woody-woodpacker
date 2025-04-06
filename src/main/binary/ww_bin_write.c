/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:37:09 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:39:25 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>

t_ww_error	ww_bin_write(t_ww_binary *bin)
{
	if (!bin)
		return (ww_err_str(ERROR_INTERNAL, "NULL provided to ww_bin_write\n"));
	if (bin->handler && bin->handler->write)
	{
		ww_trace("Writing %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->write(bin->handler, bin));
	}
	return (ww_err_fmt(ERROR_INTERNAL, "No 'write' handler for %s\n",
			bin->input));
}
