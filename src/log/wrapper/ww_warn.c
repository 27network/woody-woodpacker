/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_warn.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:42:31 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/13 23:43:16 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_LOG_INTERNAL__
#include <ww/log.h>

void	ww_warn(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ww_vlog(LOG_WARN, fmt, args);
	va_end(args);
}
