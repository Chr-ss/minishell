/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_child_dup.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 14:34:39 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/07 22:08:46 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

void	execute_child_dup_read(t_msdata *data, t_cmd *cmd)
{
	if (cmd->pipefd[RD])
	{
		debugger (BLU "cmd->pipefd[RD] %d duped to -> %d\n" RESET, \
			cmd->pipefd[RD], STDIN_FILENO);
	}
	if (cmd->pipefd[RD] && cmd->infd < 1 \
	&& dup2(cmd->pipefd[RD], STDIN_FILENO) == -1)
		error("dup error child cmd->pipefd[RD] to stdin", data);
	if (cmd->pipefd[RD] && close(cmd->pipefd[RD]) == -1)
		error("close error cmd->pipefd[RD]", data);
	if (cmd->pipefd[WR] && close(cmd->pipefd[WR]) == -1)
		error("close error cmd->pipefd[WR]", data);
}

void	execute_child_dup_write(t_msdata *data, t_cmd *cmd)
{
	if (cmd->pipe->pipefd[WR])
	{
		debugger (BLU "cmd->pipe->pipefd[WR] %d duped to -> %d\n" RESET, \
			cmd->pipefd[WR], STDOUT_FILENO);
	}
	if (cmd->pipe->pipefd[WR] && \
		cmd->outfd < 1 && dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO) == -1)
		error("dup error pipe->pipefd[WR] to stdout", data);
	if (cmd->pipe->pipefd[WR] && close(cmd->pipe->pipefd[WR]) == -1)
		error("close error cmd->pipe->pipefd[WR]", data);
	if (cmd->pipe->pipefd[RD] && close(cmd->pipe->pipefd[RD]) == -1)
		error("close error cmd->pipe->pipefd[RD]", data);
}

int	execute_child_dup(t_msdata *data, t_cmd *cmd)
{
	int	ret;

	ret = execute_child_dup_fd(data, cmd);
	if (ret == -1)
		return (1);
	if (!(data->cmd_head == cmd))
		execute_child_dup_read(data, cmd);
	if (cmd->pipe != NULL)
		execute_child_dup_write(data, cmd);
	return (0);
}
