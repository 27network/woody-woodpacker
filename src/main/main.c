/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:22:53 by kiroussa          #+#    #+#             */
/*   Updated: 2025/08/09 15:20:30 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ww/log.h"
#include <ww/binary.h>
#include <ww/cli.h>
#include <ww/consts.h>

static void	ww_dump_args(t_ww_args *args)
{
	ww_bin_handler_registry_dump();
	ww_debug("struct s_ww_args {\n");
	ww_debug("\ttarget: %s\n", args->target);
	ww_debug("\toutput: %s\n", args->output);
	ww_debug("\tencryption_algo: %s\n",
		ww_encryption_algo_str(args->encryption_algo));
	ww_debug("\tencryption_key: %s\n", args->encryption_key);
	ww_debug("\tlog_level: %s\n", ww_log_level_str(args->log_level));
	ww_debug("\tcompression: %s\n",
		ww_compression_algo_str(args->compression_algo));
	ww_debug("\tpayload_file: %s\n", args->payload_file);
	ww_debug("\tpayload_async: %d\n", args->payload_async);
	ww_debug("}\n");
}

static int	ww_process(t_ww_args *args)
{
	t_ww_binary		bin;
	t_ww_error		err;

	err = ww_bin_init(&bin, args);
	if (!err.type)
		err = ww_bin_identify(&bin);
	if (!err.type)
		err = ww_bin_read(&bin);
	if (!err.type)
		err = ww_bin_process(&bin);
	if (!err.type)
		err = ww_bin_write(&bin);
	if (err.type)
		ww_err_print(err);
	else
		ww_info("Successfully packed '%s' into '%s'\n", args->target,
			args->output);
	ww_err_release(err);
	ww_bin_free(&bin);
	return (!!err.type);
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
	ww_dump_args(&args);
	return (ww_process(&args));
}
