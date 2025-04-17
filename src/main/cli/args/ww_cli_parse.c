/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:32:14 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/17 09:20:31 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>
#include <unistd.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>
#include <ww/consts.h>

static inline int	ww_cli_opt_invalid(t_opt_globals *globals, t_opt_args *args)
{
	if (ft_strchr(args->optstring, globals->optopt))
		ww_error("-%c: option requires an argument\n", globals->optopt);
	else
		ww_error("-%c: unrecognized option\n", globals->optopt);
	ww_cli_opt_help(args->argv, STDERR_FILENO);
	return (CLI_EXIT_FAILURE);
}

static inline int	ww_cli_opt_payload_async(t_ww_args *args)
{
	args->payload_async = true;
	return (CLI_SUCCESS);
}

static inline int	ww_cli_opt_handle(t_ww_args *args, int opt,
				t_opt_globals *opt_globals)
{
	if (opt == 'c')
		return (ww_cli_opt_compression(args, opt_globals->optarg));
	if (opt == 'e')
		return (ww_cli_opt_encryption_algo(args, opt_globals->optarg));
	if (opt == 'k')
		return (ww_cli_opt_encryption_key(args, opt_globals->optarg));
	if (opt == 'o')
		return (ww_cli_opt_output(args, opt_globals->optarg));
	if (opt == 's')
		return (ww_cli_opt_signature(args, opt_globals->optarg));
	if (opt == 'p')
		return (ww_cli_opt_payload(args, opt_globals->optarg));
	if (opt == 'v')
		return (ww_cli_opt_verbose(args));
	if (opt == 'a')
		return (ww_cli_opt_payload_async(args));
	ww_error("Unrecognized option: '%c'\n", opt);
	return (CLI_EXIT_FAILURE);
}

static inline int	ww_cli_args_handle(t_ww_args *args,
				t_opt_globals *opt_globals, t_opt_args *opt_args)
{
	const int	argc = opt_args->argc - opt_globals->optind;
	const char	**argv = (const char **) opt_args->argv + opt_globals->optind;

	if (ww_cli_opt_output(args, args->output) != CLI_SUCCESS)
		return (CLI_EXIT_FAILURE);
	ww_trace("Parsing arguments (argc: %d)\n", argc);
	ww_trace("Parsing arguments (argv[0]: %s)\n", argv[0]);
	if (argc != 1)
	{
		ft_dprintf(STDERR_FILENO, USAGE_LINE, opt_args->argv[0]);
		ft_dprintf(STDERR_FILENO, "Try '%s --help' for more information.\n",
			opt_args->argv[0]);
		return (CLI_EXIT_FAILURE);
	}
	args->target = argv[0];
	return (CLI_SUCCESS);
}

int	ww_cli_parse(t_ww_args *args, int argc, char **argv)
{
	t_opt_args		opt_args;
	t_opt_globals	opt_globals;
	int				opt;
	int				ret;

	opt_globals = ft_opt_globals();
	opt_globals.opterr = 0;
	opt_args = ft_opt_args(argc, argv, OPT_POSIX | OPT_BASH_LIKE, OPTSTRING);
	ret = CLI_SUCCESS;
	while (ret == CLI_SUCCESS)
	{
		opt = ft_opt_get(&opt_globals, &opt_args);
		if (opt == -1)
			break ;
		if (opt == '?')
			return (ww_cli_opt_invalid(&opt_globals, &opt_args));
		if (opt == 'h' || opt == HELP_OPT)
			return (ww_cli_opt_help(argv, STDOUT_FILENO));
		if (opt == 'V')
			return (ww_cli_opt_version(STDOUT_FILENO));
		ret = ww_cli_opt_handle(args, opt, &opt_globals);
	}
	if (ret == CLI_SUCCESS)
		ret = ww_cli_args_handle(args, &opt_globals, &opt_args);
	return (ret);
}
