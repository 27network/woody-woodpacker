/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload.inc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:00:00 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/21 18:54:54 by kiroussa         ###   ########.fr       */
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

t_content_source	*Func(ww_bin_elf_payload)(t_ww_elf_handler *self,
						t_elf_segment *segment, size_t offset)
{
	const char		*payload = Func(g_payload);
	const size_t	payload_size = sizeof(Func(g_payload));

	return (NULL);
}

