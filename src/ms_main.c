/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_main.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/05 00:02:57 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	if (argc > 1)
		ms_error("Invalid argument count.");
	(void) argv;
	ms_parsing(&data);
	buffer = ms_readline(&data);
	printf("MAIN: %s", buffer);
	ms_error("No, error.");
	return (0);
}
