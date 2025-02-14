/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:38:18 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 14:38:27 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>

const char	*ww_getenv(char **envp, const char *env, char *fallback)
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
