/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_output.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:58:38 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 14:43:46 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

int	ww_cli_opt_output(t_ww_args *args, const char *output)
{
	int	fd;

	if (!output || !*output)
	{
		ww_error("Invalid output file: '%s'\n", output);
		return (CLI_EXIT_FAILURE);
	}
	fd = open(output, O_RDONLY);
	if (fd == -1)
	{
		if (errno == ENOENT)
		{
			args->output = output;
			return (CLI_SUCCESS);
		}
		ww_error("Could not open output file '%s': %m\n", output);
		return (CLI_EXIT_FAILURE);
	}
	close(fd);
	ww_error("Output file '%s' already exists\n", output);
	return (CLI_EXIT_FAILURE);
}
