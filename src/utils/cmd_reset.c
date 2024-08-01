/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_reset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/04 13:45:17 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/01 17:12:48 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cmd_close_fd(int *fd)
{
	close(*fd);
	*fd = 0;
}

// TODO: this doesn't free the whole structure?
void	free_cmd(t_cmd *cmd)
{
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->argv)
		free_char_array(cmd->argv);
	if (cmd->heredoc)
		free_char_array(cmd->heredoc);
	if (cmd->infd > 0)
		cmd_close_fd(&cmd->infd);
	if (cmd->outfd > 0)
		cmd_close_fd(&cmd->outfd);
	if (cmd->pipefd[0] > 0)
		cmd_close_fd(&cmd->pipefd[0]);
	if (cmd->pipefd[1] > 0)
		cmd_close_fd(&cmd->pipefd[1]);
}

void	cmd_clear(t_msdata *data)
{
	t_cmd	*tempcmd;
	t_cmd	*freelast;

	tempcmd = data->cmd_head;
	while (tempcmd)
	{
		free_cmd(tempcmd);
		freelast = tempcmd;
		tempcmd = tempcmd->pipe;
		free(freelast);
	}
	data->cmd_head = NULL;
	data->cmd_curr = NULL;
	data->pos = 0;
}

void	cmd_reset(t_msdata *data)
{
	cmd_clear(data);
	data->cmd_head = ft_calloc(sizeof(t_cmd), 1);
	if (!data->cmd_head)
		error("cmd_reset: malloc error.", data);
	data->cmd_curr = data->cmd_head;
	data->pos = 0;
	data->overrule_exit = false;
}
