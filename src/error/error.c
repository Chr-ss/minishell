/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:52:00 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/11 13:42:04 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error(char *msg, t_msdata *data)
{
	free_all(data);
	perror(msg);
	exit(EXIT_FAILURE);
}

void	check_tty(void)
{
	if (!isatty(STDIN_FILENO))
	{
		write (2, "minishell: stdin: not a tty\n", 28);
		exit (EXIT_FAILURE);
	}
	if (!isatty(STDOUT_FILENO))
	{
		write (2, "minishell: stdout: not a tty\n", 29);
		exit (EXIT_FAILURE);
	}
	if (!isatty(STDERR_FILENO))
	{
		write (2, "minishell: stderr: not a tty\n", 29);
		exit (EXIT_FAILURE);
	}
}
