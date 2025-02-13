/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_encryption.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:50:03 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:50:24 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

int	ww_cli_opt_encryption(t_ww_args *args, const char *encryption_key)
{
	if (!encryption_key)
	{
		ww_error("Invalid encryption key: '%s'\n", encryption_key);
		return (CLI_EXIT_FAILURE);
	}
	args->encryption_key = encryption_key;
	return (CLI_SUCCESS);
}
