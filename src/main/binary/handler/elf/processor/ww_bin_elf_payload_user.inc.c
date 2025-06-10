/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_user.inc.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:26:24 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/10 21:41:01 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

FASTCALL char	*Func(ww_bin_elf_payload_user_error)(
	const char *type,
	Elf(Off) *user_payload_size
) {
	ww_warn("failed to %s user payload: %m\n", type);
	ww_warn("no payload will be used\n");
	*user_payload_size = 0;
	return (NULL);
}

FASTCALL char	*Func(ww_bin_elf_payload_user_file)(
	t_ww_binary *bin,
	int *user_payload_fd;
	Elf(Off) *user_payload_size
) {
	char *user_payload;
	Elf(Off) size;

	*user_payload_fd = open(bin->args->payload_file, O_RDONLY);
	if (*user_payload_fd == -1)
		return (Func(ww_bin_elf_payload_user_error)("open", user_payload_size));
	struct stat	st;
	if (fstat(*user_payload_fd, &st) != 0)
		return (Func(ww_bin_elf_payload_user_error)("fstat", user_payload_size));
	size = (Elf(Off)) st.st_size;
	user_payload = mmap(NULL, size, PROT_READ, MAP_PRIVATE, *user_payload_fd, 0);
	if (user_payload == MAP_FAILED)
	{
		ww_warn("failed to mmap payload file: %m\n");
		ww_warn("no payload will be used\n");
		*user_payload_size = 0;
		return (NULL);
	}
	*user_payload_size = size;
	return (user_payload);
}

FASTCALL char	*Func(ww_bin_elf_payload_user)(
	t_ww_binary *bin,
	int user_payload_fd,
	Elf(Off) *user_payload_size
) {
	char *user_payload;

	if (bin->args->payload_file == NULL || ft_strcmp(bin->args->payload_file, "none") == 0)
	{
		*user_payload_size = 0;
		return (NULL);
	}
	if (ft_strcmp(bin->args->payload_file, "<built-in>") == 0)
	{
		*user_payload_size = sizeof(Func(g_default_user_payload));
		user_payload = ft_calloc(*user_payload_size, 1);
		if (user_payload)
			ft_memcpy(user_payload, Func(g_default_user_payload),
				*user_payload_size);
		return (user_payload);
	}
	return (Func(ww_bin_elf_payload_user_file)(bin, user_payload_fd, user_payload_size));
}
