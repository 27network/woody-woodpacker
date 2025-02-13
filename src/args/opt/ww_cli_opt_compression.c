/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_opt_compression.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:30:41 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:38:21 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

static const char	*g_available_compressions[] = {
	"none",
};

static void	ww_compressions_print(void)
{
	size_t			i;
	const size_t	len = sizeof(g_available_compressions)
		/ sizeof(g_available_compressions[0]);

	ww_info("Available compression algorithms:\n");
	i = 0;
	while (i < len)
	{
		ww_info("\t- %s\n", g_available_compressions[i]);
		i++;
	}
}

int	ww_cli_opt_compression(t_ww_args *args, const char *compression)
{
	size_t			i;
	const size_t	len = sizeof(g_available_compressions)
		/ sizeof(g_available_compressions[0]);

	i = 0;
	while (i < len)
	{
		if (!ft_strcmp(compression, g_available_compressions[i]))
			break ;
		i++;
	}
	if (i == len)
	{
		ww_error("Invalid compression algorithm: '%s'\n", compression);
		ww_compressions_print();
		return (CLI_EXIT_FAILURE);
	}
	args->compression = compression;
	return (CLI_SUCCESS);
}
