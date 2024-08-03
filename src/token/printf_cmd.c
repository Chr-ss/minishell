/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   printf_cmd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:05:57 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/03 11:17:15 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	printf_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	debugger(CYN "\n\t ## CMD ## \n" RESET);
	while (cmd)
	{
		debugger(BLU "CMD:%s\n" RESET, cmd->cmd);
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			debugger(BLU "ARGV[%d]:%s\n" RESET, i, cmd->argv[i]);
			i++;
		}
		debugger(BLU "INFILE:%d\n" RESET, cmd->infd);
		debugger(BLU "OUTFILE:%d\n" RESET, cmd->outfd);
		i = 0;
		while (cmd->heredoc && cmd->heredoc[i])
		{
			debugger(BLU "HEREDOC[%d]:%s\n" RESET, i, cmd->heredoc[i]);
			i++;
		}
		cmd = cmd->pipe;
		debugger("\n\n");
	}
}
