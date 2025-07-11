/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_encryption_algo.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 01:52:13 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/11 23:05:53 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/encrypt.h>
#include <ww/util.h>

enum e_ww_encryption_algo	ww_encryption_algo(const char *algo)
{
	static const char	*algos[] = {"none", "xor", "aes128"};
	static const size_t	len = sizeof(algos) / sizeof(algos[0]);
	size_t				i;

	i = 0;
	while (i < len)
	{
		if (!ww_strcmp_ignorecase(algo, algos[i]))
			break ;
		i++;
	}
	if (i == len)
		return (-1);
	return (i);
}

const char	*ww_encryption_algo_str(enum e_ww_encryption_algo algo)
{
	static const char	*algos[] = {
	[ENCRYPTION_ALGO_NONE] = "none",
	[ENCRYPTION_ALGO_XOR] = "xor",
	[ENCRYPTION_ALGO_AES128] = "aes128"
	};

	if (algo < 0 || algo >= _ENCRYPTION_ALGO_SIZE)
		return ("unknown");
	return (algos[algo]);
}
