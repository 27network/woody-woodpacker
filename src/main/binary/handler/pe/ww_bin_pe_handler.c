/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_bin_pe_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:34:12 by kiroussa          #+#    #+#             */
/*   Updated: 2025/07/18 10:19:00 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/binary/pe.h>

[[gnu::constructor]]
static void	ww_bin_pe_handler_register(void)
{
	ww_bin_handler_register((t_ww_binary_handler){
		.type = "PE",
		.struct_size = sizeof(t_ww_binary_handler),
		.identify = &ww_bin_pe_identifier,
	});
}
