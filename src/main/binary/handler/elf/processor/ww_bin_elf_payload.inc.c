/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload.inc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/22 11:22:13 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if ELF_BITNESS == 32
# define PAYLOAD_FILE "src/shellcode/elf/x86/entry/entrypoint.bin"
#elif ELF_BITNESS == 64
# define PAYLOAD_FILE "src/shellcode/elf/x86_64/entry/entrypoint.bin"
#else
# error "Invalid ELF_BITNESS"
#endif

static const char	Func(g_payload)[] = {
#embed PAYLOAD_FILE 
};
#undef PAYLOAD_FILE

//TODO: Alloc entry payload with enough size to hold the `payload` and `segments_content`
FASTCALL char	*Func(ww_bin_elf_payload_raw)(Elf(Word) *payload_size)
{
	char	*payload = ft_calloc();

	if (!payload)
		return (NULL);
	*payload_size = sizeof(Func(g_payload));
	return (Func(g_payload));
}

/**
 * The structure that holds the payload features.
 *
 * @note	This structure references the entry payload variables directly,
 * 			see `src/shellcode/elf/common/entry/variables.inc.s`.
 */
__attribute__((packed))
struct s_payload_features
{
	Elf(Word)	start_offset;
	char		loader_async;
	Elf(Word)	payload_size;
	char		encryption_key[64];
	Elf(Word)	segments_content_size;
	Elf(Word)	segments_write_offset;
	Elf(Word)
	char		*segments_content;
	char		*payload;
};

t_content_source	*Func(ww_bin_elf_payload)(
	t_ww_binary *bin,
	t_ww_elf_handler *self,
	t_elf_segment *segment,
	size_t offset
) {
	// Setup payload features
	struct s_payload_features	features = {0};
	features.loader_async = bin->args->payload_async,

	// Get payload
	features.payload = Func(ww_bin_elf_payload_raw)(&features.payload_size);
	if (!features.payload)
		return (NULL);

	// Write the primary payload features at the end of the payload
	char *payload_end = features.payload + features.payload_size;
	struct s_payload_features *variables_ptr = payload_end - sizeof(features);
	*variables_ptr = features;

	// Overwrite the dynamic buffers (`segments_content` and `payload`)
	if (bin->args->encryption_algo != WW_ENCRYPTION_NONE)
	{
		//TODO:
		memcpy(variables_ptr->encryption_key, bin->args->encryption_key, sizeof(variables_ptr->encryption_key));
	}

	char *payload_target = &variables_ptr->segments_content + variables_ptr->segments_content_size;


	return (NULL);
}

