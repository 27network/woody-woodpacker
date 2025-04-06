/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_strerror.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:19:06 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 18:20:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

const char	*elfstream_strerror(enum e_elfstream_error error)
{
	static const char	*errors[] = {
	[ELFSTREAM_OK] = "Success",
	[ELFSTREAM_ERROR] = "Unknown Error",
	[ELFSTREAM_INVALID] = "Invalid",
	[ELFSTREAM_EOF] = "End of File",
	};

	if (error < 0 || error >= _ELFSTREAM_ERROR_MAX)
		return (errors[ELFSTREAM_ERROR]);
	return (errors[error]);
}
