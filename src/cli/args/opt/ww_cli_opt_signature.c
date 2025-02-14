/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_signature.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:28:40 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:29:08 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

int	ww_cli_opt_signature(t_ww_args *args, const char *signature)
{
	args->signature = signature;
	return (CLI_SUCCESS);
}
