/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_content_size.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 22:56:01 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/09 22:56:26 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elfstream.h>

size_t	elfstream_content_size(t_content_source *content)
{
	size_t				size;

	size = 0;
	while (content)
	{
		size += content->size;
		content = content->next;
	}
	return (size);
}
