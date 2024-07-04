/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   clearcmd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/04 13:45:17 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/04 14:32:43 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


//free_char_array
static void	freecmd(t_cmd *cmd)
{
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->heredoc)
		free_char_array(cmd->heredoc);
	if (cmd->infd > 0)
	{
		close(cmd->infd);
		cmd->infd = 0;
	}
	if (cmd->outfd > 0)
	{
		close(cmd->outfd);
		cmd->outfd = 0;
	}
}

void	clearcmd(t_msdata *data)
{
	t_cmd	*tempcmd;
	t_cmd	*freelast;

	tempcmd = data->cmd_head;
	while (tempcmd)
	{
		freecmd(tempcmd);
		freelast = tempcmd;
		tempcmd = tempcmd->pipe;
		free(freelast);
	}
	data->cmd_head = ft_calloc(sizeof(t_cmd), 1);
	data->cmd_curr = data->cmd_head;
	data->pos = 0;
}
