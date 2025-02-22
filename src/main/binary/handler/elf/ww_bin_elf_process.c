/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:01:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/22 02:04:12 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/elf.h>

static const char	g_shellcode[] = {
	#embed "../../../../shellcode/elf/x86_64/payload.bin"
};

bool	ww_bin_elf_process(t_ww_elf_handler *self, t_ww_binary *bin)
{
	(void)self;
	(void)bin;
	(void)g_shellcode;
	return (WW_ERROR);
}
