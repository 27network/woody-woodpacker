/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_output.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:58:38 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:41:26 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

// Note: We intentionally don't add open's `mode` argument,
// as to make it fail if the file already exists.
int	ww_cli_opt_output(t_ww_args *args, const char *output)
{
	int	fd;

	if (!output || !*output)
	{
		ww_error("Invalid output file: '%s'\n", output);
		return (CLI_EXIT_FAILURE);
	}
	fd = open(output, O_WRONLY | O_CREAT, 0);
	if (fd == -1)
	{
		ww_error("Could not open output file '%s': %m\n", output);
		return (CLI_EXIT_FAILURE);
	}
	close(fd);
	args->output = output;
	return (CLI_SUCCESS);
}
