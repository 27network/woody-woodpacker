/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_strerror.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:30:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:00:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/error.h>

const char	*ww_strerror(enum e_ww_error_type type)
{
	static const char	*errors[] = {
	[ERROR_NONE] = "No error",
	[ERROR_ALLOC] = "Memory allocation error",
	[ERROR_INTERNAL] = "Internal error",
	[ERROR_IO] = "IO error",
	};

	if (type & ~ERROR_META_MASK)
		return (errors[type & ~ERROR_META_MASK]);
	return ("Unknown error");
}
