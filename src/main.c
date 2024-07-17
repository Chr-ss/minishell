/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/17 14:03:43 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	(void)argv;
	if (argc > 1)
		error("Invalid argument count.", &data);
	// check_tty();
	init_signal();
	initdata(&data, envp);
	input_handling(&data);
	rl_clear_history();
	exit(data.exit_code);
	return (data.exit_code);
}
