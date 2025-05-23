/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_content_free.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 23:35:08 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/12 20:29:09 by kiroussa         ###   ########.fr       */
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
		if (tmp->type == CONTENT_SOURCE_MEMORY && tmp->s_memory.allocated)
			free((void *) tmp->s_memory.data);
		free(tmp);
	}
}
