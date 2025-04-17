/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_defaults.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:33:05 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/17 09:16:40 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <ft/string.h>
#include <ft/string/parse.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>
#include <ww/util.h>

static enum e_ww_log_level	ww_parse_level(const char *verbosity)
{
	static const char	*verbosity_names[] = {
		"NONE", "FATAL", "ERROR", "WARN", "INFO",
		"DEBUG", "TRACE", "ALL", NULL
	};
	int					i;

	i = -1;
	while (verbosity_names[++i])
	{
		if (!ww_strcmp_ignorecase(verbosity, verbosity_names[i]))
			return ((enum e_ww_log_level)i);
	}
	ww_warn("Invalid verbosity level '%s', defaulting to INFO\n", verbosity);
	return (LOG_INFO);
}

void	ww_cli_defaults(t_ww_args *args, char **envp)
{
	const char	*verbosity;

	ft_bzero(args, sizeof(t_ww_args));
	args->output = ww_getenv(envp, "WW_OUTPUT", WW_OUTPUT_DEFAULT);
	args->encryption_key = ww_getenv(envp, "WW_ENCRYPTION_KEY", NULL);
	ww_cli_opt_encryption_algo(args,
		ww_getenv(envp, "WW_ENCRYPTION_ALGO", WW_ENCRYPTION_DEFAULT));
	verbosity = ww_getenv(envp, "WW_LOG_LEVEL", "INFO");
	args->log_level = ww_parse_level(verbosity);
	args->signature = ww_getenv(envp, "WW_SIGNATURE", WW_SIGNATURE_DEFAULT);
	ww_cli_opt_compression(args,
		ww_getenv(envp, "WW_COMPRESSION", NULL));
	ww_cli_opt_payload(args,
		ww_getenv(envp, "WW_PAYLOAD_FILE", NULL));
}
