/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   printf_cmd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:05:57 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/08 14:50:32 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	printf_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	debugger("\n\t ## CMD ## \n");
	while (cmd)
	{
		debugger("CMD:%s\n", cmd->cmd);
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			debugger("ARGV[%d]:%s\n", i, cmd->argv[i]);
			i++;
		}
		// if (cmd->infd)
		debugger("INFILE:%d\n", cmd->infd);
		debugger("OUTFILE:%d\n", cmd->outfd);
		i = 0;
		while (cmd->heredoc && cmd->heredoc[i])
		{
			debugger("HEREDOC[%d]:%s\n", i, cmd->heredoc[i]);
			i++;
		}
		cmd = cmd->pipe;
		debugger("\n\n");
	}
}

