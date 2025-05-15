/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:22:53 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/15 02:31:58 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary.h>
#include <ww/cli.h>
#include <ww/consts.h>

static void	ww_dump_args(t_ww_args *args)
{
	ww_bin_handler_registry_dump();
	ww_trace("struct s_ww_args {\n");
	ww_trace("\ttarget: %s\n", args->target);
	ww_trace("\toutput: %s\n", args->output);
	ww_trace("\tencryption_algo: %s\n",
		ww_encryption_algo_str(args->encryption_algo));
	ww_trace("\tencryption_key: %s\n", args->encryption_key);
	ww_trace("\tlog_level: %s\n", ww_log_level_str(args->log_level));
	ww_trace("\tcompression: %s\n",
		ww_compression_algo_str(args->compression));
	ww_trace("\tpayload_file: %s\n", args->payload_file);
	ww_trace("\tpayload_async: %d\n", args->payload_async);
	ww_trace("}\n");
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
