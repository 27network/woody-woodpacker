/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_encryption_algo.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:30:41 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 02:02:24 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

static void	ww_encryptions_print(void)
{
	size_t			i;

	ww_info("Available encryption algorithms:\n");
	i = 0;
	while (i < _ENCRYPTION_ALGO_SIZE)
	{
		ww_info("\t- %s\n", ww_encryption_algo_str(i));
		i++;
	}
}

int	ww_cli_opt_encryption_algo(t_ww_args *args, const char *encryption_algo)
{
	enum e_ww_encryption_algo	algo;

	if (!encryption_algo)
		return (CLI_EXIT_FAILURE);
	algo = ww_encryption_algo(encryption_algo);
	if (algo == (enum e_ww_encryption_algo) - 1)
	{
		ww_error("Invalid encryption algorithm: '%s'\n", encryption_algo);
		ww_encryptions_print();
		return (CLI_EXIT_FAILURE);
	}
	args->encryption_algo = algo;
	return (CLI_SUCCESS);
}
