/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_child.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 14:35:07 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/05 18:50:29 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

int	execute_check_builtin_fd_functions(t_cmd *cmd, int len)
{
	if (!ft_strncmp("echo", cmd->cmd, 4) && len == 4)
		return (1);
	else if (!ft_strncmp("cd", cmd->cmd, 2) && len == 2)
		return (1);
	else if (!ft_strncmp("env", cmd->cmd, 3) && len == 3)
		return (1);
	else if (!ft_strncmp("export", cmd->cmd, 6) && len == 6)
		return (1);
	else if (!ft_strncmp("pwd", cmd->cmd, 3) && len == 3)
		return (1);
	else if (!ft_strncmp("unset", cmd->cmd, 5) && len == 5)
		return (1);
	else if (!ft_strncmp("exit", cmd->cmd, 4) && len == 4)
		return (1);
	else
		return (0);
	return (0);
}

int	execute_check_builtin_fd(t_msdata *data, t_cmd *cmd, int len)
{
	if (execute_check_builtin_fd_functions(cmd, len) \
	&& cmd->pipe == NULL && cmd == data->cmd_head)
	{
		data->org_stdin = dup2(STDIN_FILENO, FDMAX - 1);
		if (data->org_stdin == -1)
			error("dup error stdin to data struct", data);
		data->org_stdout = dup2(STDOUT_FILENO, FDMAX - 2);
		if (data->org_stdout == -1)
			error("dup error stdout to data struct", data);
		if (execute_child_dup_fd(data, cmd) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

int	execute_check_builtin(t_msdata *data, t_cmd *cmd)
{
	int	len;
	int	ret;

	len = ft_strlen(cmd->cmd);
	ret = execute_check_builtin_fd(data, cmd, len);
	if (ret == -1)
		return (1);
	if (!ft_strncmp("echo", cmd->cmd, 4) && len == 4)
		return (echo(data, cmd->argv));
	else if (!ft_strncmp("cd", cmd->cmd, 2) && len == 2)
		return (cd(data, cmd->argv));
	else if (!ft_strncmp("env", cmd->cmd, 3) && len == 3)
		return (env(data, cmd->argv));
	else if (!ft_strncmp("export", cmd->cmd, 6) && len == 6)
		return (export(data, cmd->argv));
	else if (!ft_strncmp("pwd", cmd->cmd, 3) && len == 3)
		return (pwd());
	else if (!ft_strncmp("unset", cmd->cmd, 5) && len == 5)
		return (unset(data, cmd->argv, NULL));
	else if (!ft_strncmp("exit", cmd->cmd, 4) && len == 4)
		return (mini_exit(data, cmd->argv, 0));
	return (-1);
}

void	execute_child(t_msdata *data, t_cmd *cmd)
{
	char	*path_cmd;
	int		ret;

	path_cmd = NULL;
	ret = execute_check_builtin(data, cmd);
	if (ret > -1)
		exit(ret);
	ret = execute_path(cmd->cmd, data, &path_cmd);
	if (ret == -1)
		error("execute_child execute path error", data);
	else if (ret == 1)
	{
		free_all(data);
		exit(127);
	}
	if (add_command_to_argv(&cmd, &path_cmd) == -1)
		error("add command to argv malloc error", data);
	ret = execve(path_cmd, cmd->argv, data->envp);
	debugger("execve ret: %d\n", ret);
	free_all(data);
	free(path_cmd);
	exit(0);
}
