/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:52:00 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 17:00:07 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error(char *msg, t_msdata *data)
{
	cmd_free(data);
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->envp)
	{
		free_char_array(data->envp);
		data->envp = NULL;
	}
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
