/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_payload.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:24:52 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/02 18:15:05 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

static inline int	ww_cli_opt_payload_check(char *buf)
{
	if (memcmp(buf, ELFMAG, SELFMAG) == 0)
		ww_debug("Payload file is an ELF file\n");
	else if (memcmp(buf, "#!/", 3) == 0)
		ww_debug("Payload file is an interpretable file\n");
	else
		ww_debug("No signature found in payload file, assuming shellcode\n");
	return (CLI_SUCCESS);
}

int	ww_cli_opt_payload(t_ww_args *args, const char *payload)
{
	int		fd;
	char	buf[4];
	ssize_t	nread;

	if (!payload || !*payload)
	{
		ww_error("Invalid payload file: '%s'\n", payload);
		return (CLI_EXIT_FAILURE);
	}
	fd = open(payload, O_RDONLY);
	if (fd == -1)
	{
		ww_error("Could not open payload file '%s': %m\n", payload);
		return (CLI_EXIT_FAILURE);
	}
	nread = read(fd, buf, 4);
	close(fd);
	if (nread == -1)
		ww_error("Could not read payload file '%s': %m\n", payload);
	else if (nread != 4)
		ww_error("Invalid payload file '%s': expected at least 4 bytes\n");
	else
		return (ww_cli_opt_payload_check(buf));
	return (CLI_EXIT_FAILURE);
}
