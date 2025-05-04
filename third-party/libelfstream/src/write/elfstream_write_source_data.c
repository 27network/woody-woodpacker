/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_write_source_data.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:17:56 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/03 18:07:11 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

enum e_elfstream_error	elfstream_write_source_data(
	t_content_source *self,
	int fd
) {
	ssize_t	last_write;
	size_t	written;

	written = 0;
	while (written < self->size)
	{
		last_write = write(fd, self->data + written, self->size - written);
		if (last_write == -1)
			return (ELFSTREAM_IO);
		written += last_write;
	}
	return (ELFSTREAM_OK);
}
