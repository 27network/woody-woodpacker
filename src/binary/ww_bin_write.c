/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:37:09 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:16:55 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>

enum e_ww_error	ww_bin_write(t_ww_binary *bin)
{
	if (bin && bin->handler && bin->handler->write)
	{
		ww_trace("Writing %s with %s\n", bin->input, bin->handler->type);
		return (bin->handler->write(bin->handler, bin));
	}
	return (WW_ERROR);
}
