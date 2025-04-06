/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err_fmt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:48:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:25:49 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <ft/print.h>
#include <ft/string.h>
#include <ww/error.h>

#define ERR_ALLOC_MSG "error allocating memory for error message string"
#define ERR_ALLOC_MSG_ADD ", was: "

static inline t_ww_error	strip(t_ww_error err)
{
	err.cached_errno = 0;
	return (err);
}

t_ww_error	ww_err_fmt(enum e_ww_error_type type, const char *fmt, ...)
{
	static char	fallback[(1024 * 4) + 64];
	va_list		args;
	char		*msg;

	if (ft_strchr(fmt, '%') == NULL)
		return (ww_err_str(type, fmt));
	va_start(args, fmt);
	msg = NULL;
	ft_vasprintf(&msg, fmt, args);
	va_end(args);
	if (!msg)
	{
		if (ft_strlen(msg) < 1024)
		{
			ft_memset(fallback, 0, sizeof(fallback));
			ft_strlcpy(fallback, ERR_ALLOC_MSG ERR_ALLOC_MSG_ADD, 1024);
			ft_strlcat(fallback, fmt, 1024);
			return (strip(ww_err_str(ERROR_ALLOC, fallback)));
		}
		return (strip(ww_err_str(ERROR_ALLOC, ERR_ALLOC_MSG)));
	}
	return (ww_err_str(type | ERROR_FMT, msg));
}
