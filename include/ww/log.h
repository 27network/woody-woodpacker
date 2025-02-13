/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:48:08 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:06:17 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H
# undef LOG_H
# ifndef __WW_LOG_H__
#  define __WW_LOG_H__

/**
 * @file	log.h
 * @brief	A standardized interface for logging messages with specific levels.
 */

#  include <ft/print.h>
#  include <stdarg.h>
#  include <stdbool.h>

#  define WW_LOG_TARGET_FD 2 
#  define WW_LOG_DEFAULT_LEVEL LOG_INFO

/**
 * @brief	The possible logging levels.
 *
 * @note	`LOG_NONE` and `LOG_ALL` are not log targets, they are used
 * 			to set the global log level, to have absolute toggle on/off
 * 			values.
 */
enum e_ww_log_level
{
	LOG_NONE = 0,
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
	LOG_TRACE,
	LOG_ALL,
};

#  ifdef __WW_LOG_INTERNAL__

/**
 * @brief	The log handler for all levels, taking a va_list as argument.
 *
 * @note	You probably definitely don't want to call this, refer to
 *			`ww_log` or one of the dedicated wrappers instead
 *			(`ww_info`, `ww_error`, etc..).
 *
 * @param	level	The logging level.
 * @param	fmt		The format string.
 * @param	args	A `va_list` of the arguments.
 */
void
ww_vlog(enum e_ww_log_level level, const char *fmt, va_list args);

#  endif // __WW_LOG_INTERNAL__

/**
 * @brief	A logging function, taking the level directly, a format string,
 * 			and its argument as variadic arguments.
 *
 * @note	If you know the log level at compile time, prefer using
 * 			the dedicated wrapper functions (`ww_info`, `ww_error`, etc..)
 * 			instead.
 *
 * @param	level	The logging level.
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_log(enum e_ww_log_level level, const char *fmt, ...)
__attribute__((format(printf, 2, 3)));

/**
 * @brief	Logs a fatal error message.
 *
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_fatal(const char *fmt, ...)
__attribute__((format(printf, 1, 2)));

/**
 * @brief	Logs an error message.
 *
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_error(const char *fmt, ...)
__attribute__((format(printf, 1, 2)));

/**
 * @brief	Logs a warning message.
 *
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_warn(const char *fmt, ...)
__attribute__((format(printf, 1, 2)));

/**
 * @brief	Logs an info message.
 *
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_info(const char *fmt, ...)
__attribute__((format(printf, 1, 2)));

/**
 * @brief	Logs a debug message.
 *
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_debug(const char *fmt, ...)
__attribute__((format(printf, 1, 2)));

/**
 * @brief	Logs a trace message.
 *
 * @param	fmt		The format string.
 * @param	...		The arguments.
 */
void
ww_trace(const char *fmt, ...)
__attribute__((format(printf, 1, 2)));

/**
 * @brief	Sets the global log level.
 *
 * @param	level	The log level to set.
 */
void
ww_log_set_level(enum e_ww_log_level level);

/**
 * @brief	Gets the global log level.
 *
 * @return	A pointer to the program log level.
 */
enum e_ww_log_level
*ww_log_level(void);

/**
 * @brief	Gets the string representation of a log level.
 *
 * @param	level	The log level to get the string representation of.
 *
 * @return	A pointer to the string representation of the log level.
 */
const char
*ww_log_level_str(enum e_ww_log_level level);

# endif // __WW_LOG_H__
#endif // LOG_H
