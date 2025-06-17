/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compress.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:14:05 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/17 12:40:54 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPRESS_H
# define COMPRESS_H
# undef COMPRESS_H
# ifndef __WW_COMPRESS_H__
#  define __WW_COMPRESS_H__

/**
 * @brief	The compression algorithms available.
 */
enum e_ww_compression_algo
{
	COMPRESSION_ALGO_NONE = 0,
	COMPRESSION_ALGO_SMLZ,
	_COMPRESSION_ALGO_SIZE,
};

enum e_ww_compression_algo
ww_compression_algo(const char *algo);

const char
*ww_compression_algo_str(enum e_ww_compression_algo algo);

# endif // __WW_COMPRESS_H__
#endif // COMPRESS_H
