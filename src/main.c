/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/13 14:22:39 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	if (argc > 1)
		error("Invalid argument count.");
	initdata(&data, argv, envp); // malloc >> data->cmd_head, data->envp
	input_handling(&data);
	error("No, error.");
	return (0);
}
