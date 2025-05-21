/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_payload.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:24:52 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/21 17:52:34 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <ft/mem.h>
#include <ft/string.h>
#include <unistd.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

static inline int	ww_cli_opt_payload_check(t_ww_args *args, char *buf,
						const char *payload_file)
{
	if (ft_memcmp(buf, ELFMAG, SELFMAG) == 0)
		ww_debug("Payload file is an ELF file\n");
	else if (ft_memcmp(buf, "#!/", 3) == 0)
		ww_debug("Payload file is an interpretable file\n");
	else
		ww_debug("No signature found in payload file, assuming shellcode\n");
	args->payload_file = payload_file;
	return (CLI_SUCCESS);
}

int	ww_cli_opt_payload(t_ww_args *args, const char *payload_file)
{
	int		fd;
	char	buf[4];
	ssize_t	nread;

	if (!payload_file)
		return (CLI_EXIT_FAILURE);
	if (ft_strcmp(payload_file, "none") == 0)
		args->payload_file = NULL;
	if (ft_strcmp(payload_file, "none") == 0)
		return (CLI_SUCCESS);
	fd = open(payload_file, O_RDONLY);
	if (fd == -1)
		ww_error("Could not open payload file '%s': %m\n", payload_file);
	if (fd == -1)
		return (CLI_EXIT_FAILURE);
	nread = read(fd, buf, 4);
	close(fd);
	if (nread == -1)
		ww_error("Could not read payload file '%s': %m\n", payload_file);
	else if (nread != 4)
		ww_error("Invalid payload file '%s': expected at least 4 bytes\n",
			payload_file);
	else
		return (ww_cli_opt_payload_check(args, buf, payload_file));
	return (CLI_EXIT_FAILURE);
}
