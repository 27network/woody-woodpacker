/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 09:47:43 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 09:59:09 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <ww/error.h>

t_ww_error	ww_err_str(enum e_ww_error_type type, const char *str)
{
	return ((t_ww_error){
		.type = type,
		.data = (void *) str,
		.cached_errno = errno
	});
}
