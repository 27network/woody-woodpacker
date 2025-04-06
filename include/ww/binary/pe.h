/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pe.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:36:17 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 11:58:55 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PE_H
# define PE_H
# undef PE_H
# ifndef __WW_BINARY_PE_H__
#  define __WW_BINARY_PE_H__

#  include <stdint.h>
#  include <ww/binary.h>

#  define PE_MAGIC "MZ"

typedef struct s_ww_pe_dos_header
{
	uint16_t	e_magic;
	uint16_t	e_cblp;
	uint16_t	e_cp;
	uint16_t	e_crlc;
	uint16_t	e_cparhdr;
	uint16_t	e_minalloc;
	uint16_t	e_maxalloc;
	uint16_t	e_ss;
	uint16_t	e_sp;
	uint16_t	e_csum;
	uint16_t	e_ip;
	uint16_t	e_cs;
	uint16_t	e_lfarlc;
	uint16_t	e_ovno;
	uint16_t	e_res[4];
	uint16_t	e_oemid;
	uint16_t	e_oeminfo;
	uint16_t	e_res2[10];
	int32_t		e_lfanew;
}	t_ww_pe_dos_header;

typedef struct s_ww_pe_file_header
{
	uint16_t	machine;
	uint16_t	nsects;
	uint32_t	timestamp;
	uint32_t	symtab_offset;
	uint32_t	nsyms;
	uint16_t	opthdr_size;
	uint16_t	characteristics;
}	t_ww_pe_file_header;

typedef struct s_ww_pe_nt_header
{
	uint32_t			signature;
	t_ww_pe_file_header	file_header;
	uint16_t			optional_header_size;
}	t_ww_pe_nt_header;

t_ww_error	ww_bin_pe_identifier(t_ww_binary *bin, bool *identified);

# endif // __WW_BINARY_PE_H__
#endif // PE_H
