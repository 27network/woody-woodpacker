/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_log_level.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:04:25 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 00:11:06 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ww/log.h>

enum e_ww_log_level	*ww_log_level(void)
{
	static enum e_ww_log_level	level = WW_LOG_DEFAULT_LEVEL;

	return (&level);
}

void	ww_log_set_level(enum e_ww_log_level new_level)
{
	if (new_level > LOG_ALL || new_level < 0)
		return ;
	*ww_log_level() = new_level;
}
