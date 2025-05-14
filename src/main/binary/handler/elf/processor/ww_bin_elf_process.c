/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:01:30 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/15 00:32:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <ww/binary/elf.h>

FASTCALL bool	ww_bin_elf_segment_is_load(t_elfstream *stream,
						t_elf_segment *segment)
{
	if (stream->bitness == ELFSTREAM_32)
		return (segment->phdr32.p_type == PT_LOAD);
	return (segment->phdr64.p_type == PT_LOAD);
}

FASTCALL bool	ww_bin_elf_segment_is_further(t_elfstream *stream,
						t_elf_segment *segment, t_elf_segment *origin)
{
	if (stream->bitness == ELFSTREAM_32)
		return (segment->phdr32.p_vaddr >= origin->phdr32.p_vaddr);
	return (segment->phdr64.p_vaddr >= origin->phdr64.p_vaddr);
}

FASTCALL void	ww_bin_elf_entry(t_elfstream *stream, t_elf_segment *segment,
					size_t offset)
{
	if (stream->bitness == ELFSTREAM_32)
	{
		segment->phdr32.p_flags |= PF_X;
		stream->ehdr32.e_entry = segment->phdr32.p_vaddr + offset;
	}
	else
	{
		segment->phdr64.p_flags |= PF_X;
		stream->ehdr64.e_entry = segment->phdr64.p_vaddr + offset;
	}
}

FASTCALL t_elf_segment	*ww_bin_elf_target_segment(t_ww_elf_handler *self)
{
	t_elfstream		*stream;
	t_elf_segment	*target;
	t_elf_segment	*tmp;
	size_t			i;

	target = NULL;
	stream = &self->stream;
	i = 0;
	while (i < stream->segment_count)
	{
		tmp = stream->segments + i;
		if (ww_bin_elf_segment_is_load(stream, tmp))
		{
			if (!target || ww_bin_elf_segment_is_further(stream, tmp, target))
				target = tmp;
		}
		i++;
	}
	return (target);
}

static const char	g_payload_x86[] = {
#embed "src/shellcode/elf/x86/entrypoint.bin"
};

static const char	g_payload_x86_64[] = {
#embed "src/shellcode/elf/x86_64/entrypoint.bin"
};

#define START_OFFSET	

t_content_source	*ww_bin_elf_payload(t_ww_elf_handler *self)
{
	t_content_source	*payload_data;
	char				*payload;

	if (self->stream.bitness == ELFSTREAM_32)
		payload = ft_calloc(sizeof(g_payload_x86), sizeof(char));
	else
		payload = ft_calloc(sizeof(g_payload_x86_64), sizeof(char));
	if (!payload)
		return (NULL);
	if (self->stream.bitness == ELFSTREAM_32)
	{
		ft_memcpy(payload, g_payload_x86, sizeof(g_payload_x86));
		// 1 byte for payload (to be filled)
		// 1 byte for loader_fork boolean
		// 4 bytes for payload_size
		// 4 bytes for start_addr [target]
		ft_memcpy(payload + sizeof(g_payload_x86) - 2 - 4 - 4, &self->stream.ehdr32.e_entry, sizeof(Elf32_Addr));
	}
	else
	{
		ft_memcpy(payload, g_payload_x86_64, sizeof(g_payload_x86_64));
		// Same but 8 bytes instead of 4 for Elf(Addr)
		// TODO: Change this function to be a bitness-sensitive function
		ft_memcpy(payload + sizeof(g_payload_x86_64) - 2 - 8 - 8, &self->stream.ehdr64.e_entry, sizeof(Elf64_Addr));
	}
	payload_data = elfstream_source_data(payload, sizeof(g_payload_x86_64), true);
	if (payload_data)
		return (payload_data);
	free(payload);
	return (NULL);
}

t_ww_error	ww_bin_elf_process(t_ww_elf_handler *self, t_ww_binary *bin)
{
	t_content_source	*payload_data;
	t_elf_segment		*target;
	size_t				location;

	(void) bin;
	ww_trace("%s: Adding new binary data to ELF.\n", __func__);
	target = ww_bin_elf_target_segment(self);
	if (!target)
		return (ww_err_fmt(ERROR_INTERNAL, "failed to find target segment"));
	payload_data = ww_bin_elf_payload(self);
	if (!payload_data)
		return (ww_err_fmt(ERROR_ALLOC, "failed to allocate payload data"));
	location = elfstream_segment_append(&self->stream, target, payload_data);
	ww_bin_elf_entry(&self->stream, target, location);
	ww_info("Injected binary data at %#x\n", (unsigned int) location);
	return (ww_ok());
}
