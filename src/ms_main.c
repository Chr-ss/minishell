/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_main.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/24 20:24:31 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include <stdio.h>

void	ms_error(char *msg)
{
	perror(msg);
	exit(1);
}

int	main(int argc, char **argv)
{
	char *buffer;

	if (argc > 1)
		ms_error("Invalid argument count.");
	(void) argv;
	while (1)
	{
		buffer = readline("minishell:~$");
		if (!buffer)
			ms_error("readline malloc error.");
		printf("%s\n", buffer);
	}
	return (0);
}
