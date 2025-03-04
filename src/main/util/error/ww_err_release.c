/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_err_release.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 03:54:20 by kiroussa          #+#    #+#             */
/*   Updated: 2025/03/03 03:54:47 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/error.h>

void	ww_err_release(t_ww_error err)
{
	if (err.type & ERROR_FMT == ERROR_FMT)
		ft_strdel((char *) &err.data);
}
