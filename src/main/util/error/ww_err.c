/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:48:17 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 09:52:11 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <ww/error.h>

t_ww_error	ww_err(enum e_ww_error_type type)
{
	return ((t_ww_error){
		.type = type,
		.data = NULL,
		.cached_errno = errno
	});
}
