/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_compression.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:30:41 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 02:02:43 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

static void	ww_compressions_print(void)
{
	size_t			i;

	ww_info("Available compression algorithms:\n");
	i = 0;
	while (i < _COMPRESSION_ALGO_SIZE)
	{
		ww_info("\t- %s\n", ww_compression_algo_str(i));
		i++;
	}
}

int	ww_cli_opt_compression(t_ww_args *args, const char *compression)
{
	enum e_ww_compression_algo	algo;

	if (!compression)
		return (CLI_EXIT_FAILURE);
	algo = ww_compression_algo(compression);
	if (algo == (enum e_ww_compression_algo) - 1)
	{
		ww_error("Invalid compression algorithm: '%s'\n", compression);
		ww_compressions_print();
		return (CLI_EXIT_FAILURE);
	}
	args->compression = algo;
	return (CLI_SUCCESS);
}
