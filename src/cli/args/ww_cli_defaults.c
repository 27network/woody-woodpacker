/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_cli_defaults.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:33:05 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 02:07:15 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <ft/string.h>
#include <ft/string/parse.h>
#define __WW_CLI_INTERNAL__
#include <ww/cli.h>

/**
 * @brief	Gets the value from the environment table of a given variable via
 *			its name/key.
 *
 * @note	This is way less efficient than doing a constant-time lookup via
 *			a hash-table, but it doesn't require any memory allocation.
 *
 * @param	envp		The environment table.
 * @param	env			The name/key of the variable (e.g. "WW_ENCRYPTION_KEY").
 * @param	fallback	The default value to return if the variable is not set.
 *
 * @return	The value of the variable, or `fallback` if it is not found.
 */
static char	*ww_getenv(char **envp, const char *env, char *fallback)
{
	int		i;
	size_t	target_len;
	size_t	env_len;

	i = -1;
	target_len = ft_strlen(env);
	while (envp && envp[++i])
	{
		if (!ft_strchr(envp[i], '='))
			continue ;
		env_len = ft_strcspn(envp[i], "=");
		if (env_len != target_len)
			continue ;
		if (!ft_strncmp(envp[i], env, target_len))
			return (envp[i] + target_len + 1);
	}
	return (fallback);
}

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
		if (!ft_strcmp(verbosity, verbosity_names[i]))
			return ((enum e_ww_log_level)i);
	}
	ww_warn("Invalid verbosity level '%s', defaulting to INFO\n", verbosity);
	return (LOG_INFO);
}

void	ww_cli_defaults(t_ww_args *args, char **envp)
{
	char		*verbosity;

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
}
