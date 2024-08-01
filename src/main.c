/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/01 17:14:12 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// uncomment check_tty for tester
int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	(void)argv;
	if (argc > 1)
		error("Invalid argument count.", &data);
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
