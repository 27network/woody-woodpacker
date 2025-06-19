/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_write_source_data.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:17:56 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/19 17:27:40 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>
#include <unistd.h>
#include <sys/types.h>

// Note: if s_memory.data is NULL, this function will return ELFSTREAM_OK
// without doing anything, since the callsite will lseek to `curr + size`,
// effectively writing NULL bytes `size` times.
enum e_elfstream_error	elfstream_write_source_data(
	t_content_source *self,
	int fd
) {
	ssize_t	last_write;
	size_t	written;

	if (!self->s_memory.data)
	{
		off_t curr = lseek(fd, 0, SEEK_CUR);
		if (curr == (off_t) -1)
			return (ELFSTREAM_IO);
		if (lseek(fd, curr + self->size - 1, SEEK_SET) == (off_t) -1)
			return (ELFSTREAM_IO);
		if (write(fd, "", 1) != 1)
			return (ELFSTREAM_IO);
		return (ELFSTREAM_OK);
	}
	written = 0;
	while (written < self->size)
	{
		last_write = write(fd, self->s_memory.data + written,
				self->size - written);
		if (last_write == -1)
			return (ELFSTREAM_IO);
		written += last_write;
	}
	return (ELFSTREAM_OK);
}
