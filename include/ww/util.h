/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:35:54 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 20:50:56 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H
# undef UTIL_H
# ifndef __WW_UTIL_H__
#  define __WW_UTIL_H__

#  include <ft/io.h>
#  include <ft/string.h>
#  include <stddef.h>

#  define smartstr __attribute__((cleanup(ft_strdel))) char *
#  define smartfd __attribute__((cleanup(ft_closep))) int

/**
 * @brief	Compares two strings, ignoring case, up to `n` characters.
 *
 * @param	s1	The first string.
 * @param	s2	The second string.
 * @param	n	The maximum number of characters to compare.
 *
 * @return	The result of the comparison.
 */
int
ww_strncmp_ignorecase(const char *s1, const char *s2, size_t n);

/**
 * @brief	Compares two strings, ignoring case, up to `n` characters.
 *
 * @param	s1	The first string.
 * @param	s2	The second string.
 *
 * @return	The result of the comparison.
 */
int
ww_strcmp_ignorecase(const char *s1, const char *s2);

/**
 * @brief	Gets the value from the environment table of a given variable via
 *			its name/key.
 *
 * @note	This is way less efficient than doing a constant-time lookup via
 *			a hash-table, but it doesn't require any memory allocation.
 *
 * @param	envp		The environment table.
 * @param	env			The name/key of the variable (e.g. "WW_ENCRYPTION_KEY").
 * @param	fallback	The default value to return if the variable is not set.
 *
 * @return	The value of the variable, or `fallback` if it is not found.
 */
const char
*ww_getenv(char **envp, const char *env, const char *fallback);

# endif // __WW_UTIL_H__
#endif // UTIL_H
