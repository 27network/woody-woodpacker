/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_version.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:34:54 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 21:28:50 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/cli.h>
#include <ww/consts.h>

#define FOSS_SPEECH "\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law\n\
"

/**
 * License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law
	*/

int	ww_cli_opt_version(int fd)
{
	ft_putstr_fd(WW_PROJECT_NAME " " WW_PROJECT_VERSION "\n"
		WW_PROJECT_URL "\n", fd);
	ft_putstr_fd("Copyright (c) 2025  jsauvain, kiroussa\n\n", fd);
	ft_putstr_fd("License ISC: <https://www.isc.org/>\n", fd);
	ft_putstr_fd(FOSS_SPEECH, fd);
	return (CLI_EXIT_SUCCESS);
}
