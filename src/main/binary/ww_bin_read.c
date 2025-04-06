/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:36:38 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:44:30 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <ww/binary.h>

t_ww_error	ww_bin_read(t_ww_binary *bin)
{
	t_ww_error	err;

	errno = 0;
	if (!bin)
		return (ww_err_str(ERROR_INTERNAL, "NULL provided to ww_bin_read\n"));
	if (bin->handler && bin->handler->read)
	{
		err = ww_bin_map(bin);
		if (err.type)
			return (err);
		ww_trace("Reading %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->read(bin->handler, bin));
	}
	return (ww_err_fmt(ERROR_INTERNAL, "No 'read' handler for %s\n",
			bin->input));
}
