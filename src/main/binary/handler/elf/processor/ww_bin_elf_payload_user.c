/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_elf_payload_user.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 15:26:24 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/17 16:47:46 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ww/binary/elf.h>

#ifndef ELF_BITNESS
# define ELF_BITNESS 32
# define USER_PAYLOAD_FILE "src/shellcode/elf/x86/payload/payload.bin"
# include "ww_bin_elf_payload_user.c"
# define ELF_BITNESS 64
# define USER_PAYLOAD_FILE "src/shellcode/elf/x86_64/payload/payload.bin"
# include "ww_bin_elf_payload_user.c"
#else // ELF_BITNESS
# include <elfstream_macros.h>

# define PAYLOAD_MAX_SIZE (1024 * 1024 * 500)

static const char	Func(g_default_user_payload)[] = {
# embed USER_PAYLOAD_FILE
};

FASTCALL char	*Func(ww_bin_elf_payload_user_error)(
	const char *type,
	const char *desc,
	Elf(Off) *user_payload_size
) {
	if (desc)
		ww_warn("%s\n", desc);
	else
		ww_warn("failed to %s user payload: %m\n", type);
	ww_warn("no payload will be used\n");
	*user_payload_size = 0;
	return (NULL);
}

FASTCALL char	*Func(ww_bin_elf_payload_user_file)(
	t_ww_binary *bin,
	Elf(Off) *user_payload_size
) {
	char *user_payload = NULL;
	char *user_payload_map = NULL;
	struct stat st;
	smartfd user_payload_fd = -1;

	if (stat(bin->args->payload_file, &st) != 0)
		return (Func(ww_bin_elf_payload_user_error)("stat", NULL, user_payload_size));
	if (S_ISDIR(st.st_mode) || !S_ISREG(st.st_mode))
		return (Func(ww_bin_elf_payload_user_error)(NULL, "user payload file is not a regular file", user_payload_size));
	if (st.st_size == 0)
		return (Func(ww_bin_elf_payload_user_error)(NULL, "user payload file is empty", user_payload_size));
	if (st.st_size > PAYLOAD_MAX_SIZE)
		return (Func(ww_bin_elf_payload_user_error)(NULL, "user payload file is too large", user_payload_size));
	*user_payload_size = (Elf(Off)) st.st_size;
	user_payload_fd = open(bin->args->payload_file, O_RDONLY);
	if (user_payload_fd == -1)
		return (Func(ww_bin_elf_payload_user_error)("open", NULL, user_payload_size));
	user_payload_map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, user_payload_fd, 0);
	if (user_payload_map == MAP_FAILED)
		return (Func(ww_bin_elf_payload_user_error)("mmap", NULL, user_payload_size));
	user_payload = ft_calloc(*user_payload_size, 1);
	if (user_payload)
		ft_memcpy(user_payload, user_payload_map, *user_payload_size);
	munmap(user_payload_map, *user_payload_size);
	ww_trace("Loaded custom user payload\n");
	return (user_payload);
}

char	*Func(ww_bin_elf_payload_user)(
	t_ww_binary *bin,
	Elf(Off) *user_payload_size
) {
	char *user_payload = NULL;

	if (bin->args->payload_file == NULL || ft_strcmp(bin->args->payload_file, "none") == 0)
	{
		ww_trace("Loaded empty user payload\n");
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
		ww_trace("Loaded builtin user payload\n");

		int fd = open("woody-user-payload.bin", O_WRONLY | O_CREAT, 0755);
		if (fd == -1)
			return (Func(ww_bin_elf_payload_user_error)("open", NULL, user_payload_size));
		write(fd, user_payload, *user_payload_size);
		close(fd);

		return (user_payload);
	}
	return (Func(ww_bin_elf_payload_user_file)(bin, user_payload_size));
}

# undef USER_PAYLOAD_FILE
# undef ELF_BITNESS
#endif
