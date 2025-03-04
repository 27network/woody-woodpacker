/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err_fmt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:48:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/03/03 03:53:44 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <ft/string.h>
#include <ww/error.h>

#define ERR_ALLOC_MSG "error allocating memory for error message string"
#define ERR_ALLOC_MSG_ADD ", was: "

static inline t_ww_error	ww_err_str(size_t type,
								const char *str)
{
	return ((t_ww_error){
		.type = (enum e_ww_error_type) type,
		.data = (void *) str
	});
}

t_ww_error	ww_err_fmt(enum e_ww_error_type type, const char *fmt, ...)
{
	static char	fallback[1024 + 64];
	va_list		args;
	const char	*msg;

	if (ft_strchr(fmt, '%') == NULL)
		return (ww_err_str(type, fmt));
	va_start(args, fmt);
	msg = ft_vasprintf(fmt, args);
	va_end(args);
	if (!msg)
	{
		if (ft_strlen(msg) < 1024)
		{
			ft_memset(fallback, 0, sizeof(fallback));
			ft_strlcpy(fallback, ERR_ALLOC_MSG ERR_ALLOC_MSG_ADD, 1024);
			ft_strlcat(fallback, msg, 1024);
			return (ww_err_str(ERROR_ALLOC, fallback));
		}
		return (ww_err_str(ERROR_ALLOC, ERR_ALLOC_MSG));
	}
	return (ww_err_str(type | ERROR_FMT, msg));
}
