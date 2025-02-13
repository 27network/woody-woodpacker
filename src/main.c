/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:22:53 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:42:32 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/cli.h>
#include <ww/consts.h>

static void	ww_dump_args(t_ww_args *args)
{
	ww_debug("struct s_ww_args {\n");
	ww_debug("\ttarget: %s\n", args->target);
	ww_debug("\toutput: %s\n", args->output);
	ww_debug("\tencryption_key: %s\n", args->encryption_key);
	ww_debug("\tlog_level: %s\n", ww_log_level_str(args->log_level));
	ww_debug("\tsignature: '%s'\n", args->signature);
	ww_debug("\tcompression: %s\n", args->compression);
	ww_debug("}\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_ww_args	args;
	int			ret;

	ww_cli_defaults(&args, envp);
	ww_log_set_level(args.log_level);
	ret = ww_cli_parse(&args, argc, argv);
	if (ret != CLI_SUCCESS)
		return (ret - 1);
	ww_log_set_level(args.log_level);
	ww_info("Hello, "WW_PROJECT_NAME"!\n");
	ww_dump_args(&args);
	return (0);
}
