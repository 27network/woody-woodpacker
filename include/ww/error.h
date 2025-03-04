/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:11:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/03/03 03:56:31 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# undef ERROR_H
# ifndef __WW_ERROR_H__
#  define __WW_ERROR_H__

/**
 * @file error.h
 * @brief A flexible error type system.
 */

#  define ERROR_MAX 2
#  define ERROR_META_MAX 3

enum e_ww_error_type
{
	ERROR_NONE = 0b0000,
	ERROR_ALLOC = 0b0001,
	ERROR_FMT = 0b0010,
};

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
