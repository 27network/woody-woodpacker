/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 10:14:45 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 10:15:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENCRYPT_H
# define ENCRYPT_H
# undef ENCRYPT_H
# ifndef __WW_ENCRYPT_H__
#  define __WW_ENCRYPT_H__

/**
 * @brief	The encryption algorithms available.
 */
enum e_ww_encryption_algo
{
	ENCRYPTION_ALGO_NONE = 0,
	ENCRYPTION_ALGO_XOR,
	ENCRYPTION_ALGO_AES, // jordan gros malade va
	_ENCRYPTION_ALGO_SIZE,
};

enum e_ww_encryption_algo
ww_encryption_algo(const char *algo);

const char
*ww_encryption_algo_str(enum e_ww_encryption_algo algo);

# endif // __WW_ENCRYPT_H__
#endif // ENCRYPT_H
