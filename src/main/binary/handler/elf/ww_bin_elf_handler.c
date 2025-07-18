/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:57:12 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/18 10:12:48 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

// We provide intentionally wrong signatures for better DX (preventing a cast)
// in the target functions (s_ww_binary_handler* -> s_ww_elf_handler*).
// It's a pointer either way, so it's fine.

t_ww_error	ww_bin_elf_free(t_ww_binary_handler *self, t_ww_binary *bin);
t_ww_error	ww_bin_elf_identifier(t_ww_binary *bin, bool *is_elf);
t_ww_error	ww_bin_elf_process(t_ww_binary_handler *self,
				t_ww_binary *bin);
t_ww_error	ww_bin_elf_read(t_ww_binary_handler *self,
				t_ww_binary *bin);
t_ww_error	ww_bin_elf_write(t_ww_binary_handler *self,
				t_ww_binary *bin);

[[gnu::constructor]]
static void	ww_bin_elf_handler_register(void)
{
	ww_bin_handler_register((t_ww_binary_handler){
		.type = "ELF",
		.struct_size = sizeof(t_ww_elf_handler),
		.free = &ww_bin_elf_free,
		.identify = &ww_bin_elf_identifier,
		.process = &ww_bin_elf_process,
		.read = &ww_bin_elf_read,
		.write = &ww_bin_elf_write,
	});
}
