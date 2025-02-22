/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ww_strncmp_ignorecase.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:37:13 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/14 14:39:41 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string.h>

int	ww_strncmp_ignorecase(const char *s1, const char *s2, size_t n)
{
	if (!n--)
		return (0);
	while (*s1 && *s2 && n && ft_tolower(*s1) == ft_tolower(*s2))
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
