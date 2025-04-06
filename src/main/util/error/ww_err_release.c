/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err_release.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:54:20 by kiroussa          #+#    #+#             */
/*   Updated: 2025/04/06 09:58:46 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>
#include <ww/error.h>

void	ww_err_release(t_ww_error err)
{
	if ((err.type & ERROR_FMT) == ERROR_FMT)
		ft_strdel((char **) &err.data);
}
