/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test-payload.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:48:01 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/02 16:06:07 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * 0x7F E L F   =>  memfd_create
 * #!			=>  memfd_create
 * else			=>  jmp <code>
 */

static void	exec(char *file, bool fork)
{
	int	fd = open(file, O_RDONLY);
}

int	main(int argc, char **argv)
{
	int		target;
	bool	fork;

	fork = false;
	target = 1;
	if (argc != 2)
	{
		fprintf(stderr, "%s [fork] <file_to_exec>\n", argv[0]);
		return (1);
	}
	if (strcmp(argv[1], "fork") == 0)
	{
		fork = true;
		target++;
	}
	exec(argv[target], fork);
	return (0);
}
