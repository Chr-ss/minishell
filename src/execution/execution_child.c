/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_child.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 14:35:07 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/25 13:32:18 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

extern pid_t	g_pid;

int	execute_check_builtin(t_msdata *data, t_cmd *cmd)
{
	int	len;

	len = ft_strlen(cmd->cmd);
	if (!ft_strncmp("echo", cmd->cmd, len))
		return (echo(data, cmd->argv));
	else if (!ft_strncmp("cd", cmd->cmd, len))
		return (cd(data, cmd->argv));
	else if (!ft_strncmp("env", cmd->cmd, len))
		return (env(data, cmd->argv));
	else if (!ft_strncmp("export", cmd->cmd, len))
		return (export(data, cmd->argv));
	else if (!ft_strncmp("pwd", cmd->cmd, len))
		return (pwd());
	else if (!ft_strncmp("unset", cmd->cmd, len))
		return (unset(data, cmd->argv, NULL));
	else if (!ft_strncmp("exit", cmd->cmd, len))
		return (mini_exit(data, cmd->argv, 0));
	else
		return (-1);
	return (-1);
}

void	execute_child_minishell(t_msdata *data, t_cmd *cmd)
{
	int		len;

	len = ft_strlen(cmd->cmd);
	len -= 9;
	if (len < 0)
		return ;
	if (!(ft_strncmp("minishell", cmd->cmd + len, 9)))
		init_signal(data, true, true);
	return ;
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
		error("execute_child execute path error\n", data);
	else if (ret == 1)
	{
		write(2, "minishell: command not found\n", 29);
		free_all(data);
		exit(127);
	}
	if (add_command_to_argv(&cmd, &path_cmd) == -1)
		error("add command to argv malloc error\n", data);
	ret = execve(path_cmd, cmd->argv, data->envp);
	debugger("execve ret: %d\n", ret);
	free_all(data);
	free(path_cmd);
	exit(0);
}
