/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_ok.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:46:55 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 09:52:24 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/error.h>

t_ww_error	ww_ok(void)
{
	return ((t_ww_error){
		.type = ERROR_NONE,
		.data = NULL,
		.cached_errno = 0
	});
}
