/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/02 13:38:31 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// uncomment check_tty for tester
int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	(void)argv;
	if (argc > 1)
	{
		write(1, "Invalid argument count.\n", 24);
		return (1);
	}
	if (!TEST)
		check_tty();
	debugger("pid %d\n", getpid());
	init_signal(NULL, false);
	initdata(&data, envp);
	input_handling(&data);
	rl_clear_history();
	exit(data.exit_code);
	return (data.exit_code);
}
