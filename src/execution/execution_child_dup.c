/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_child_dup.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 14:34:39 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 13:51:00 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_child_dup_fd_out(t_msdata *data, t_cmd *cmd)
{
	debugger(BLU "cmd->outfd %d duped to -> %d\n" RESET, cmd->outfd, STDOUT_FILENO);
	if (dup2(cmd->outfd, STDOUT_FILENO) == -1)
		error("dup error child outfd to stdout", data);
	debugger(BLU "closing cmd->outfd: %d\n" RESET, cmd->outfd);
	if (close(cmd->outfd) == -1)
		error("close error child outfd after dub to stdout", data);
	if (cmd->pipe != NULL)
	{
		debugger(BLU "closing cmd->pipe->pipefd[WR]: %d\n" RESET, cmd->pipe->pipefd[WR]);
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close err child RDend pipe after fd dub to stdin", data);
		cmd->pipe->pipefd[WR] = 0;
	}
	return (1);
}

int	execute_child_dup_fd_in(t_msdata *data, t_cmd *cmd)
{
	debugger(BLU "cmd->infd %d duped to -> %d\n" RESET, cmd->infd, STDIN_FILENO);
	if (dup2(cmd->infd, STDIN_FILENO) == -1)
		error("dup error child infd to stdin", data);
	debugger(BLU "closing cmd->infd: %d\n" RESET, cmd->infd);
	if (close(cmd->infd) == -1)
		error("close error child infd after dub to stdin", data);
	if (!(data->cmd_head == cmd))
	{
		debugger(BLU "closing cmd->pipefd[RD]: %d\n" RESET, cmd->pipefd[RD]);
		if (close(cmd->pipefd[RD]) == -1)
			error("close err child RDend pipe after fd dub to stdout", data);
		cmd->pipefd[RD] = 0;
	}
	return (1);
}

int	execute_child_dup_fd(t_msdata *data, t_cmd *cmd)
{
	int	ret;

	ret = 0;
	debugger(BLU "stdin %d duped to -> %d\n" RESET, STDIN_FILENO, FDMAX -1);
	data->org_stdin = dup2(STDIN_FILENO, FDMAX - 1);
	if (data->org_stdin == -1)
		error("dup error stdin to data struct", data);
	debugger(BLU "stdout %d duped to -> %d\n" RESET, STDOUT_FILENO, FDMAX -2);
	data->org_stdout = dup2(STDOUT_FILENO, FDMAX - 2);
	if (data->org_stdout == -1)
		error("dup error stdout to data struct", data);
	if (cmd->infd < 0 || cmd->outfd < 0)
	{
		return (-1);
	}
	if (cmd->infd > 0)
		ret = execute_child_dup_fd_in(data, cmd);
	if (cmd->outfd > 0)
		ret = execute_child_dup_fd_out(data, cmd);
	if (cmd->pipe)
	{
		debugger(BLU "cmd->pipe->pipefd[WR]: %d duped to -> %d\n" RESET, cmd->pipe->pipefd[WR], STDOUT_FILENO);
		if (dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO) == -1)
			error("dup error stdin to data struct", data);
	}
	return (ret);
}

int	execute_child_dup(t_msdata *data, t_cmd *cmd)
{
	int	ret;

	ret = execute_child_dup_fd(data, cmd);
	if (ret == -1)
		return (1);
	if (!(data->cmd_head == cmd))
	{
		if (cmd->pipefd[RD])
			debugger (BLU "cmd->pipefd[RD] %d duped to -> %d\n" RESET, cmd->pipefd[RD], STDIN_FILENO);
		if (cmd->pipefd[RD] && dup2(cmd->pipefd[RD], STDIN_FILENO) == -1)
			error("dup error child read end pipe to stdin", data);
		if (cmd->pipefd[RD])
			debugger (BLU "closing cmd->pipefd[RD] %d\n" RESET, cmd->pipefd[RD]);
		if (cmd->pipefd[RD] && close(cmd->pipefd[RD]) == -1)
			error("close error child read end pipe after dub to stdin", data);
	}
	if (cmd->pipe != NULL)
	{

		if (cmd->pipefd[WR])
			debugger (BLU "cmd->pipefd[WR] %d duped to -> %d\n" RESET, cmd->pipefd[WR], STDOUT_FILENO);
		if (cmd->pipefd[WR] && dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO) == -1)
			error("dup error child write end pipe to stdout", data);
	}
	return (0);
}
