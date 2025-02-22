/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:57:12 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:14:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

enum e_ww_error	ww_bin_elf_identifier(t_ww_binary *bin);
enum e_ww_error	ww_bin_elf_read(struct s_ww_binary_handler *self,
					t_ww_binary *bin);
enum e_ww_error	ww_bin_elf_process(struct s_ww_binary_handler *self,
					t_ww_binary *bin);
enum e_ww_error	ww_bin_elf_write(struct s_ww_binary_handler *self,
					t_ww_binary *bin);

__attribute__((constructor))
static void	ww_bin_elf_handler_register(void)
{
	ww_bin_handler_register((t_ww_binary_handler){
		.type = "ELF",
		.struct_size = sizeof(t_ww_elf_handler),
		.identify = &ww_bin_elf_identifier,
		.read = &ww_bin_elf_read,
		.process = &ww_bin_elf_process,
		.write = &ww_bin_elf_write,
	});
}
