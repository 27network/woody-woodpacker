/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:40:15 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/19 17:45:26 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary.h>
#include <ww/log.h>

t_ww_error	ww_bin_init(t_ww_binary *bin, t_ww_args *args)
{
	ft_memset(bin, 0, sizeof(t_ww_binary));
	bin->input = args->target;
	bin->output = args->output;
	bin->args = args;
	ww_trace("Opening input file '%s'\n", bin->input);
	bin->input_fd = open(bin->input, O_RDONLY);
	if (bin->input_fd == -1)
		return (ww_err_fmt(ERROR_IO, "Could not open input file '%s': %m\n",
				bin->input));
	return (ww_ok());
}
