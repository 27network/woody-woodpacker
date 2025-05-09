/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_write_source_fd.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:17:56 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/09 17:13:49 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

#define BUF_SIZE 65536

static inline size_t	elfstream_source_fd_read(
	t_content_source *self,
	char *buffer,
	size_t size
) {
	ssize_t	last_read;
	size_t	nread;

	DBG("reading at offset %zu", self->s_file.offset);
	if (lseek(self->s_file.fd, self->s_file.offset, SEEK_SET) == -1)
		return (ELFSTREAM_IO);
	nread = 0;
	DBG("reading %zu bytes", size);
	while (nread < size)
	{
		last_read = read(self->s_file.fd, buffer + nread, size - nread);
		if (last_read == -1)
		{
			DBG("read error: %m");
			return ((size_t) - 1);
		}
		nread += last_read;
	}
	DBG("read %zu bytes", nread);
	return (nread);
}

static inline int	elfstream_source_fd_write(
	int fd,
	char *buffer,
	size_t size
) {
	ssize_t	last_write;
	size_t	written;

	written = 0;
	while (written < size)
	{
		last_write = write(fd, buffer + written, size - written);
		if (last_write == -1)
		{
			DBG("write error: %m");
			return (-1);
		}
		written += last_write;
	}
	DBG("wrote %zu bytes", written);
	return (0);
}

enum e_elfstream_error	elfstream_write_source_fd(
	t_content_source *self,
	int fd
) {
	char	buffer[BUF_SIZE];
	size_t	nread;

	DBG("reading from fd %d", self->s_file.fd);
	nread = elfstream_source_fd_read(self, buffer, BUF_SIZE);
	if (nread == (size_t) -1)
		return (ELFSTREAM_IO);
	if (elfstream_source_fd_write(fd, buffer, nread) == -1)
		return (ELFSTREAM_IO);
	DBG("wrote required bytes");
	return (ELFSTREAM_OK);
}
