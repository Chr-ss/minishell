/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_child_dup_fd.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 14:34:39 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/04 15:55:42 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_child_dup_fd_out(t_msdata *data, t_cmd *cmd)
{
	debugger(BLU "cmd->outfd %d duped to -> %d\n" RESET, \
		cmd->outfd, STDOUT_FILENO);
	if (dup2(cmd->outfd, STDOUT_FILENO) == -1)
		error("dup error child cmd->outfd", data);
	debugger(BLU "closing cmd->outfd: %d\n" RESET, cmd->outfd);
	if (close(cmd->outfd) == -1)
		error("close error cmd->outfd", data);
	return (1);
}

int	execute_child_dup_fd_in(t_msdata *data, t_cmd *cmd)
{
	debugger(BLU "cmd->infd %d duped to -> %d\n" RESET, \
		cmd->infd, STDIN_FILENO);
	if (dup2(cmd->infd, STDIN_FILENO) == -1)
		error("dup error child cmd->infd", data);
	debugger(BLU "closing cmd->infd: %d\n" RESET, cmd->infd);
	if (close(cmd->infd) == -1)
		error("close error child cmd->infd", data);
	return (1);
}

int	execute_child_dup_fd(t_msdata *data, t_cmd *cmd)
{
	int	ret;

	ret = 0;
	if (cmd->infd < 0 || cmd->outfd < 0)
	{
		return (-1);
	}
	if (cmd->infd > 0)
		ret = execute_child_dup_fd_in(data, cmd);
	if (cmd->outfd > 0)
		ret = execute_child_dup_fd_out(data, cmd);
	return (ret);
}
