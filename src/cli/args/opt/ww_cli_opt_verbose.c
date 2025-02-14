/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_verbose.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:59:33 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:42:43 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

int	ww_cli_opt_verbose(t_ww_args *args)
{
	if (args->log_level == LOG_TRACE)
	{
		ww_error("Too many verbose flags, maximum is %d\n",
			(LOG_ALL - 1) - WW_LOG_DEFAULT_LEVEL);
		return (CLI_EXIT_FAILURE);
	}
	args->log_level += 1;
	return (CLI_SUCCESS);
}
