/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:11:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/02 12:31:44 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# undef ERROR_H
# ifndef __WW_ERROR_H__
#  define __WW_ERROR_H__

#  define ERROR_TYPE_MASK 0b00001111
#  define ERROR_META_MASK 0b11110000

enum e_ww_error_type
{
	ERROR_NONE = 0b00000000,	// No error
	ERROR_ALLOC = 0b000000010,	// Memory allocation error
	ERROR_IO = 0b000000100,		// IO error
	ERROR_FMT = 0b00010000,		// META: Has a format string, to be free'd 
};

const char
*ww_strerror(enum e_ww_error_type type);

typedef struct s_ww_error
{
	enum e_ww_error_type	type;
	void					*data;
}	t_ww_error;

t_ww_error
ww_ok(void);

t_ww_error
ww_err(enum e_ww_error_type type);

t_ww_error
ww_err_fmt(enum e_ww_error_type type, const char *fmt, ...);

void
ww_err_release(t_ww_error err);

# endif // __WW_ERROR_H__
#endif // ERROR_H
