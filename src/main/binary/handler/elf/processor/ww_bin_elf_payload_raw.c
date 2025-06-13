/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_raw.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:25:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/12 17:15:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <ww/binary/elf.h>

#ifndef ELF_BITNESS
# define ELF_BITNESS 32
# include "ww_bin_elf_payload_raw.c"
# define ELF_BITNESS 64
# include "ww_bin_elf_payload_raw.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

char	*Func(ww_bin_elf_payload_raw)(
	Elf(Off) *total_size,
) {
	char		*payload;
	Elf(Off)	payload_size;

	payload_size = sizeof(Func(g_payload));
	// We need to deduct 2 Elf(Off) from the payload size
	// to account for `payload` and `segments_content`
	payload_size -= sizeof(Elf(Off)) * 2;
	*orig_size = payload_size;
	// And then add the actual size of those buffers
	payload_size += segments_size;
	payload_size += payload_size;

	ww_trace("allocating %#x bytes for payload\n", (unsigned int)payload_size);
	ww_trace("segments_size: %#x\n", (unsigned int)segments_size);
	ww_trace("user_payload_size: %#x\n", (unsigned int)user_payload_size);

	*total_size = payload_size;
	payload = ft_calloc(payload_size, 1);
	if (payload)
		ft_memcpy(payload, Func(g_payload), sizeof(Func(g_payload)));
	return (payload);
}

# undef ELF_BITNESS
#endif
