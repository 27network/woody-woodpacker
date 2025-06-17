/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_compression_algo.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 01:51:18 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/17 12:41:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/compress.h>
#include <ww/util.h>

enum e_ww_compression_algo	ww_compression_algo(const char *algo)
{
	static const char	*algos[] = {"none", "smzl"};
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

const char	*ww_compression_algo_str(enum e_ww_compression_algo algo)
{
	static const char	*algos[] = {
	[COMPRESSION_ALGO_NONE] = "none",
	[COMPRESSION_ALGO_SMLZ] = "smlz"
	};

	if (algo < 0 || algo >= _COMPRESSION_ALGO_SIZE)
		return ("unknown");
	return (algos[algo]);
}
