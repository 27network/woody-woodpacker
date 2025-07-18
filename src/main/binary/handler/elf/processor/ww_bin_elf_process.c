/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:01:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/18 09:43:43 by kiroussa         ###   ########.fr       */
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
	if (self->stream.bitness == ELFSTREAM_32)
		return (ww_bin_elf_process_x32(self, bin));
	return (ww_bin_elf_process_x64(self, bin));
}

#else // ELF_BITNESS
# include <elfstream_macros.h>

FASTCALL bool	Func(ww_bin_elf_target_normalize)(t_elfstream *stream, t_elf_segment *target)
{
	Elf(Phdr)			*phdr;
	Elf(Off)			diff;
	t_content_source	*blob;

	phdr = (Elf(Phdr) *) &target->phdr32;
	if (phdr->p_filesz < phdr->p_memsz)
	{
		diff = phdr->p_memsz - phdr->p_filesz;
		blob = elfstream_source_data(NULL, diff, false);
		if (!blob)
			return (false);
		ww_trace("normalizing target segment by adding %#lx null bytes\n", (size_t) diff);
		phdr->p_memsz = phdr->p_filesz;
		elfstream_segment_append(stream, target, blob);
	}
	return (true);
}

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
	Elf(Off)			woody_entry;
	Elf(Off)			routines_offset = 0;
	Elf(Ehdr)			*ehdr = (Elf(Ehdr) *) &self->stream.ehdr32;

	ww_trace("Original entry point: %#lx\n", (size_t)ehdr->e_entry);
	target = Func(ww_bin_elf_target)(&self->stream);
	if (!target)
		return (ww_err_fmt(ERROR_INTERNAL, "failed to find target segment"));
	if (!Func(ww_bin_elf_target_normalize)(&self->stream, target))
		return (ww_err_fmt(ERROR_INTERNAL, "failed to normalize target segment"));
	offset = elfstream_segment_append(&self->stream, target, NULL);
	woody_entry = Func(ww_bin_elf_entry)(self, target, offset);
	ww_trace("Injecting woody_entry @ %#lx\n", (size_t)woody_entry);
	payload = Func(ww_bin_elf_payload_build)(bin, self, target, woody_entry, &routines_offset);
	if (!payload)
		return (ww_err_fmt(ERROR_ALLOC, "failed to allocate payload data"));
	(void)elfstream_segment_append(&self->stream, target, payload);
	ehdr->e_entry = woody_entry + routines_offset;
	ww_trace("New entry point: %#lx\n", (size_t)ehdr->e_entry);
	ww_debug("Done processing\n");
	return (ww_ok());
}

# undef ELF_BITNESS
#endif
