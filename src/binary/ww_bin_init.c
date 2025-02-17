/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:40:15 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/17 23:16:26 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <ft/mem.h>
#include <unistd.h>
#include <ww/binary.h>
#include <ww/log.h>

enum e_ww_error	ww_bin_init(t_ww_binary *bin, t_ww_args *args)
{
	ft_memset(bin, 0, sizeof(t_ww_binary));
	bin->input = args->target;
	bin->output = args->output;
	bin->input_fd = open(bin->input, O_RDONLY);
	if (bin->input_fd == -1)
	{
		ww_trace("Could not open input file: %m\n");
		return (WW_ERROR);
	}
	return (WW_OK);
}
