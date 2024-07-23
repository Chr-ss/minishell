/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_child_dup.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 14:34:39 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/24 00:34:35 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_child_dup_fd_out(t_msdata *data, t_cmd *cmd)
{
	if (dup2(cmd->outfd, STDOUT_FILENO) == -1)
		error("dup error child outfd to stdout", data);
	if (close(cmd->outfd) == -1)
		error("close error child outfd after dub to stdout", data);
	if (cmd->pipe != NULL)
	{
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close err child RDend pipe after fd dub to stdin", data);
	}
	return (1);
}

int	execute_child_dup_fd_in(t_msdata *data, t_cmd *cmd)
{
	if (dup2(cmd->infd, STDIN_FILENO) == -1)
		error("dup error child infd to stdin", data);
	if (close(cmd->infd) == -1)
		error("close error child infd after dub to stdin", data);
	if (!(data->cmd_head == cmd))
	{
		if (close(cmd->pipefd[RD]) == -1)
			error("close err child RDend pipe after fd dub to stdout", data);
	}
	return (1);
}

int	execute_child_dup_fd(t_msdata *data, t_cmd *cmd)
{
	int	ret;

	ret = 0;
	data->org_stdin = dup2(STDIN_FILENO, 1000);
	if (data->org_stdin == -1)
		error("dup error stdin to data struct", data);
	data->org_stdout = dup2(STDOUT_FILENO, 1001);
	if (data->org_stdout == -1)
		error("dup error stdout to data struct", data);
	if (cmd->infd < 0 || cmd->outfd < 0)
	{
		perror ("fds ");
		return (-1);
	}
	if (cmd->infd > 0)
		ret = execute_child_dup_fd_in(data, cmd);
	if (cmd->outfd > 0)
		ret = execute_child_dup_fd_out(data, cmd);
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
		if (dup2(cmd->pipefd[RD], STDIN_FILENO) == -1)
			error("dup error child read end pipe to stdin", data);
		if (close(cmd->pipefd[RD]) == -1)
			error("close error child read end pipe after dub to stdin", data);
	}
	if (cmd->pipe != NULL)
	{
		if (dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO) == -1)
			error("dup error child write end pipe to stdout", data);
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close error child write end pipe after dub to stdout", data);
	}
	return (0);
}
