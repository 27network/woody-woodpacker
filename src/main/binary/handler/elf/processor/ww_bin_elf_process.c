/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:01:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/04 13:00:46 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_BITNESS
# include <ft/mem.h>
# include <ww/binary/elf.h>

# define ELF_BITNESS 32
# include "ww_bin_elf_process.c"
# define ELF_BITNESS 64
# include "ww_bin_elf_process.c"

// Wrapper function, see below for the actual implementation
t_ww_error	ww_bin_elf_process(t_ww_elf_handler *self, t_ww_binary *bin)
{
	ww_warn("Segment compression is not implemented yet, disabling");
	bin->args->compression_algo = COMPRESSION_ALGO_NONE; //TODO: Implement

	if (self->stream.bitness == ELFSTREAM_32)
		return (ww_bin_elf_process_x32(self, bin));
	return (ww_bin_elf_process_x64(self, bin));
}

#else // ELF_BITNESS
# include <elfstream_macros.h>
# include "ww_bin_elf_target.inc.c"
# include "ww_bin_elf_payload.inc.c"
// # include "ww_bin_elf_entry.inc.c"

/**
 * This function should:
 *  - Identify the target segment
 *  - Create the appropriate payload
 *  - Inject the payload into the target segment
 *  - Set the entry point to the payload
 */
t_ww_error	Func(ww_bin_elf_process)(t_ww_elf_handler *self, t_ww_binary *bin)
{
	t_content_source	*payload;
	t_elf_segment		*target;
	size_t				offset;

	target = Func(ww_bin_elf_find_target)(&self->stream);
	if (!target)
		return (ww_err_fmt(ERROR_INTERNAL, "failed to find target segment"));
	offset = elfstream_segment_append(&self->stream, target, NULL);
	payload = Func(ww_bin_elf_payload)(bin, self, target, offset);
	if (!payload)
		return (ww_err_fmt(ERROR_ALLOC, "failed to allocate payload data"));
	(void)elfstream_segment_append(&self->stream, target, payload);
	return (ww_ok());
}

# undef ELF_BITNESS

#endif
