/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_vlog.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:49:50 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/17 12:39:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include <ft/mem.h>
#include <ft/string.h>
#include <ft/print.h>
#include <stdarg.h>
#include <stdio.h>
#include <ww/log.h>

// Some extra space next to the project name
// Allows to buffer level formatting and other pretty stuff
//
// 5 (max log level length)
// + 2 (paren)
// + 2 (':' && ' ' char)
#define WW_BUFFER_EXTRA 9

const char	*ww_log_level_str(enum e_ww_log_level level)
{
	static const char	*levels[] = {
		"none", "fatal", "error", "warn", "info",
		"debug", "trace", "all", NULL
	};

	if (level < 0 || level > LOG_ALL)
		return ("?????");
	return (levels[level]);
}

// Here we use a double buffer for writing the prefix
//
// Basically we have two buffers next to each-other, both separated/terminated
// by a null character, making two strings.
//
// The first of those strings is filled and used when writing
// a log level that's 5 chars long, the second when it's 4 chars.
//
// The buffers being right next to each-other, we can use a simple
// `+ ft_strlen` of itself to get to the second one.
static inline void	ww_log_prefix(enum e_ww_log_level level)
{
	static char	prefix[(sizeof(WW_PROJECT_NAME) + WW_BUFFER_EXTRA + 1) * 2];
	static bool	setup = false;
	size_t		copy_size;
	char		*target;

	if (!setup)
	{
		ft_bzero(prefix, WW_BUFFER_EXTRA + sizeof(WW_PROJECT_NAME));
		ft_strcpy(prefix, WW_PROJECT_NAME"(     ): ");
		ft_strcpy(prefix + ft_strlen(prefix) + 1, WW_PROJECT_NAME"(    ): ");
		setup = true;
	}
	target = prefix;
	copy_size = 5;
	if (level == LOG_WARN || level == LOG_INFO)
	{
		target += ft_strlen(target) + 1;
		copy_size--;
	}
	ft_strncpy(target + sizeof(WW_PROJECT_NAME), ww_log_level_str(level),
		copy_size);
	ft_putstr_fd(target, WW_LOG_TARGET_FD);
}

void	ww_vlog(enum e_ww_log_level level, const char *fmt, va_list args)
{
	if (level >= LOG_ALL)
		level = LOG_TRACE;
	if (level <= LOG_NONE)
		level = LOG_FATAL;
	if (*ww_log_level() < level)
		return ;
	ww_log_prefix(level);
	vdprintf(WW_LOG_TARGET_FD, fmt, args);
}
