/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:00:31 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:06:37 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <string.h>
#include <ww/error.h>
#include <ww/log.h>

void	ww_err_print(t_ww_error err)
{
	const char	*type = ww_strerror(err.type);
	const int	errno = err.cached_errno;
	const char	*msg = err.data;

	if (!msg)
	{
		if (errno)
			ww_error("%s (Errno %d: %s)\n", type, errno, strerror(errno));
		else
			ww_error("%s\n", type);
	}
	else if ((err.type & ERROR_FMT) == ERROR_FMT)
		ww_error("%s", msg);
	else
	{
		if (errno)
			ww_error("%s: %s (Errno %d: %s)\n", type, msg,
				errno, strerror(errno));
		else
			ww_error("%s: %s\n", type, msg);
	}
}
