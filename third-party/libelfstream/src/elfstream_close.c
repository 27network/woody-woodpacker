/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:10:55 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 19:11:20 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

enum e_elfstream_error	elfstream_close(t_elfstream *self)
{
	if (!self)
		return (ELFSTREAM_ERROR);
	if (self->segments)
		free(self->segments);
	if (self->fd > 2)
		close(self->fd);
	return (ELFSTREAM_OK);
}
