/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:11:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:00:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# undef ERROR_H
# ifndef __WW_ERROR_H__
#  define __WW_ERROR_H__

#  define ERROR_META_MASK 0b11110000

#  include <stddef.h>

/**
 * @brief Possible error types the program can return.
 *
 * These are separated into two categories:
 * - Effective errors, which are error types that are uniquely encountered
 *   during the program's execution.
 * - Meta errors, which act as metadata flags in combination with the
 *   effective error types.
 *
 * You can distinguish between the two by checking the type against
 * `ERROR_META_MASK`.
 *
 * Note that the effective error types are not necessarily representative
 * of all the error states of the program, but are rather a categorizing
 * subset of them; they are meant to be used as a kind of filter or
 * "error code", and are not used for the actual error message (apart from
 * some appending `errno` values, like `ERROR_IO`, for more detailed error
 * reporting).
 */
enum e_ww_error_type
{
	ERROR_NONE = 0,					// No error
	ERROR_ALLOC,					// Memory allocation error
	ERROR_IO,						// IO error
	ERROR_INTERNAL,					// Internal error
	ERROR_FMT = 0b00010000,			// META: Has a format string, to be free'd 
};

/**
 * @brief Gets a string representation of an error type, or the string
 * "Unknown error" if the type is unknown.  This is meant to be used for
 * classification purposes, and not as a standalone error message.
 *
 * @param type The error type to get the string representation of.
 *
 * @return The string representation of the error type.
 */
const char
*ww_strerror(enum e_ww_error_type type);

/**
 * @brief An error structure that contains the error type and a pointer to
 * some error data, if applicable.
 *
 * @param type			The error type. Note that this can be a composite of both
 *						effective and meta error types.
 * @param data			A pointer to the error data, if applicable, or NULL.
 * @param cached_errno	The cached errno value, if applicable.
 */
typedef struct s_ww_error
{
	enum e_ww_error_type	type;
	void					*data;
	int						cached_errno;
}	t_ww_error;

/**
 * @brief Creates an empty error structure to indicate that no error
 * occurred.
 *
 * @return An ERROR_NONE type'd error structure.
 */
t_ww_error
ww_ok(void);

/**
 * @brief Creates an error structure with the given error type with no
 * associated data.
 *
 * @param type The error type.
 *
 * @return The appropriate error structure.
 */
t_ww_error
ww_err(enum e_ww_error_type type);

/**
 * @brief Creates an error structure with the given error type and a constant
 * string as the error data.
 *
 * Note that using this function gives more leniency than using `ww_err_fmt`,
 * as you delegate part of the formatting context it, which can in turn
 * add additional information, such as `errno`, to the error message.
 *
 * @param type The error type.
 * @param str The error data.
 *
 * @return The appropriate error structure.
 */
t_ww_error
ww_err_str(enum e_ww_error_type type, const char *str);

/**
 * @brief Creates an error structure with the given error type and a formatted
 * string as the error data, as per `printf` semantics.
 *
 * Using this function gives you full control over the formatting of the output
 * string, down to the trailing newline character.  If you want the program to
 * provide more context automatically, use `ww_err_str` instead.
 *
 * @param type The error type.
 * @param fmt The format string.
 * @param ... The format string arguments.
 *
 * @return The appropriate error structure.
 */
t_ww_error
ww_err_fmt(enum e_ww_error_type type, const char *fmt, ...)
__attribute__((format(printf, 2, 3)));

/**
 * @brief Prints the given error structure to stderr.
 *
 * @param err The error structure to print.
 */
void
ww_err_print(t_ww_error err);

/**
 * @brief Frees the error data of the given error structure, if needed.
 *
 * @param err The error structure to free.
 */
void
ww_err_release(t_ww_error err);

# endif // __WW_ERROR_H__
#endif // ERROR_H
