/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/09 17:59:20 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// This function still needs to be called at some point to check if our stdin and out are tty!!
void	check_tty(void)
{
	// if (!isatty(STDIN_FILENO))
	// {
	// 	write (2, "minishell: stdin: not a tty\n", 28);
	// 	exit (1);
	// }
	if (!isatty(STDOUT_FILENO))
	{
		write (2, "minishell: stdout: not a tty\n", 29);
		exit (1);
	}
	if (!isatty(STDERR_FILENO))
	{
		write (2, "minishell: stderr: not a tty\n", 29);
		exit (1);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	if (argc > 1)
		error("Invalid argument count.");
	// check_tty();
	init_signal();
	initdata(&data, argv, envp);
	input_handling(&data);
	rl_clear_history();
	exit(data.exit_code);
	return (0);
}
