/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 14:24:16 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/12 16:54:43 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_H
# define ELF_H
# undef ELF_H
# ifndef __WW_BINARY_ELF_H__
#  define __WW_BINARY_ELF_H__

#  include <elf.h>
#  include <elfstream.h>
#  include <stdbool.h>
#  include <ww/binary.h>

typedef struct s_ww_elf_handler
{
	t_ww_binary_handler		base;
	t_elfstream				stream;
}	t_ww_elf_handler;

// Internal cross-bitness functions

t_ww_error
ww_bin_elf_process_x32(t_ww_elf_handler *self, t_ww_binary *bin);
t_ww_error
ww_bin_elf_process_x64(t_ww_elf_handler *self, t_ww_binary *bin);

t_elf_segment
*ww_bin_elf_target_x32(t_elfstream *stream);
t_elf_segment
*ww_bin_elf_target_x64(t_elfstream *stream);

t_content_source
*ww_bin_elf_payload_build_x32(t_ww_binary *bin, t_ww_elf_handler *self,
						      t_elf_segment *target, size_t offset);
t_content_source
*ww_bin_elf_payload_build_x64(t_ww_binary *bin, t_ww_elf_handler *self,
							  t_elf_segment *target, size_t offset);

char
*ww_bin_elf_payload_raw_x32(Elf32_Off *total_size, Elf32_Off *orig_size,
							Elf32_Off segments_size, Elf32_Off user_payload_size);
char
*ww_bin_elf_payload_raw_x64(Elf64_Off *total_size, Elf64_Off *orig_size,
							Elf64_Off segments_size, Elf64_Off user_payload_size);

char
*ww_bin_elf_payload_user_x32(t_ww_binary *bin, Elf32_Off *user_payload_size);
char
*ww_bin_elf_payload_user_x64(t_ww_binary *bin, Elf64_Off *user_payload_size);


# endif // __WW_BINARY_ELF_H__
#endif // ELF_H
