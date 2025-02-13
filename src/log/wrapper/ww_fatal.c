/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_fatal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:43:55 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/13 23:43:58 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_LOG_INTERNAL__
#include <ww/log.h>

void	ww_fatal(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ww_vlog(LOG_FATAL, fmt, args);
	va_end(args);
}
