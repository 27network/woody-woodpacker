/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:18:58 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 21:06:34 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_CLI_INTERNAL__
#include <ww/cli.h>
#include <ww/consts.h>

int	ww_cli_opt_help(char **argv, int fd)
{
	ft_dprintf(fd, USAGE_LINE HELP_MESSAGE, argv[0], WW_OUTPUT_DEFAULT,
		WW_ENCRYPTION_DEFAULT, WW_COMPRESSION_DEFAULT);
	return (CLI_EXIT_SUCCESS);
}
