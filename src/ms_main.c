/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_main.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 18:17:24 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_error(char *msg)
{
	perror(msg);
	exit(1);
}

int	main(int argc, char **argv)
{
	t_msdata	data;
	char		*buffer;

	if (argc > 1)
		ms_error("Invalid argument count.");
	(void) argv;
	buffer = ms_readline(&data);
//	ms_parsing(&data);
	return (0);
}
