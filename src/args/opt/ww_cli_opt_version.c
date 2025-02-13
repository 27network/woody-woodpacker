/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_version.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:34:54 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:40:45 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/cli.h>
#include <ww/consts.h>

int	ww_cli_opt_version(int fd)
{
	ft_dprintf(fd, WW_PROJECT_NAME " " WW_PROJECT_VERSION "\n"
		WW_PROJECT_URL "\n");
	return (CLI_EXIT_SUCCESS);
}
