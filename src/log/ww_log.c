/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_log.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:49:38 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/13 23:44:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_LOG_INTERNAL__
#include <ww/log.h>

void	ww_log(enum e_ww_log_level level, const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ww_vlog(level, fmt, args);
	va_end(args);
}
