/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_debug.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:41:36 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/13 23:43:49 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_LOG_INTERNAL__
#include <ww/log.h>

void	ww_debug(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ww_vlog(LOG_DEBUG, fmt, args);
	va_end(args);
}
