/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:57:42 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 12:04:02 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACHO_H
# define MACHO_H
# undef MACHO_H
# ifndef __WW_BINARY_MACHO_H__
#  define __WW_BINARY_MACHO_H__

#  include <stdint.h>
#  include <ww/binary.h>

#  define MACHO_MAGIC_32 0xfeedface
#  define MACHO_MAGIC_64 0xfeedfacf

typedef struct s_ww_macho_header
{
	uint32_t	magic;
	uint32_t	cputype;
	uint32_t	cpusubtype;
	uint32_t	filetype;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;
	uint32_t	flags;
	uint32_t	reserved;
}	t_ww_macho_header;

t_ww_error	ww_bin_macho_identifier(t_ww_binary *bin, bool *identified);

# endif // __WW_BINARY_MACHO_H__
#endif // MACHO_H
