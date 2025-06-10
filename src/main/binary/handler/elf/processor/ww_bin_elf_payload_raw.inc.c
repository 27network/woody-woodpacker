/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_raw.inc.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:25:32 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/31 15:25:39 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

FASTCALL char	*Func(ww_bin_elf_payload_raw)(
	Elf(Off) *total_size,
	Elf(Off) *orig_size,
	Elf(Off) segments_size,
	Elf(Off) user_payload_size
) {
	//char		*payload;
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
	return (ft_calloc(payload_size, 1));
}
