/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_content_free.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 23:35:08 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/11 23:35:12 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

void	elfstream_content_free(t_content_source *content)
{
	t_content_source	*tmp;

	while (content)
	{
		tmp = content;
		content = content->next;
		free(tmp);
	}
}
