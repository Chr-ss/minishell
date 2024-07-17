/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/02 12:51:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/17 14:49:14 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

bool is_child = 1;

// REFERENCE: https://reactive.so/post/42-a-comprehensive-guide-to-pipex
// REFERENCE: https://www.gnu.org/software/libc/manual/html_node/
// Testing-File-Type.html
// REFERENCE: https://janelbrandon.medium.com/understanding-the-path-variable
// -6eae0936e976

// cat < src/main.c | grep if < src/execution/execution.c
// cat < src/main.c | grep if > test
// ls -l | grep build > test | cat -e

int	execute_child_dup_fd(t_msdata *data, t_cmd *cmd)
{
	int ret;

	ret = 0;
	if (cmd->infd < 0 || cmd->outfd < 0)
		error("error in parsing, cmds not executed", data);
	if (cmd->infd > 0)
	{
		if (dup2(cmd->infd, STDIN_FILENO) == -1)
			error("dup error child infd to stdin", data);
		if (close(cmd->infd) == -1)
			error("close error child infd after dub to stdin", data);
		if (!(data->cmd_head == cmd))
		{
			if (close(cmd->pipefd[RD]) == -1)
				error("close error child read end pipe after fd dub to stdout", data);
		}
		ret = 1;
	}
	if (cmd->outfd > 0)
	{
		if (data->cmd_head == cmd)
		{
			data->org_stdout = dup(STDOUT_FILENO);
			if (data->org_stdout == -1)
				error("dup error stdout to data struct", data);
		}
		if (dup2(cmd->outfd, STDOUT_FILENO) == -1)
			error("dup error child outfd to stdout", data);
		if (close(cmd->outfd) == -1)
			error("close error child outfd after dub to stdout", data);
		if (cmd->pipe != NULL)
		{
			if (close(cmd->pipe->pipefd[WR]) == -1)
				error("close error child read end pipe after fd dub to stdin", data);
		}
		ret = 1;
	}
	return (ret);
}


void	execute_child_dup(t_msdata *data, t_cmd *cmd)
{
	int ret;

	ret = execute_child_dup_fd(data, cmd);
	if (ret == 1)
		return ;
	if (cmd->pipe != NULL)
	{
		if (dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO) == -1)
			error("dup error child write end pipe to stdout", data);
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close error child write end pipe after dub to stdout", data);
		if (close(cmd->pipe->pipefd[RD]) == -1)
			error("close error child read end pipe after dub to stdout", data);
	}
	if (!(data->cmd_head == cmd))
	{
		if (dup2(cmd->pipefd[RD], STDIN_FILENO) == -1)
			error("dup error child read end pipe to stdin", data);
		if (close(cmd->pipefd[RD]) == -1)
			error("close error child read end pipe after dub to stdin", data);
	}
}

void	execute_parent_close_pipe(t_msdata *data, t_cmd *cmd)
{
	if (!(data->cmd_head == cmd))
	{
		if (close(cmd->pipefd[RD]) == -1)
			error("close error parent read end of pipe", data);
	}
	if (cmd->pipe != NULL)
	{
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close error parent write end of pipe", data);
	}
	if (data->cmd_head == cmd)
	{
		if (data->org_stdout > 0)
		{

			if (dup2(data->org_stdout, 1) == -1)
				error("dup org_stdout to stdout", data);
			if (close(data->org_stdout) == -1)
				error("close error org_stdout", data);
		}
	}
}


int execute_check_builtin(t_msdata *data, t_cmd *cmd)
{
	int len;

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
		return (mini_exit(data, cmd->argv));
	else
		return (-1);
	return (-1);
}

void	execute_child_minishell(t_cmd *cmd)
{
	int		len;

	len = ft_strlen(cmd->cmd);
	len -= 9;
	if (len < 0)
		return ;
	if(!(ft_strncmp("minishell", cmd->cmd + len, 9)))
		is_child = 0;
	return ;
}

void	execute_child(t_msdata *data, t_cmd *cmd)
{
	char	*path_cmd;
	int		ret;

	execute_child_dup(data, cmd);
	ret = execute_check_builtin(data, cmd);
	if (ret > -1)
		exit(ret);
	ret= execute_path(cmd->cmd, data, &path_cmd);
	if (ret == -1)
		error("execute_child execute path error\n", data);
	else if (ret == 1)
		write(2, "minishell: Command not found\n", 29);
	if (add_command_to_argv(&cmd, &path_cmd) == -1)
		error("add command to argv malloc error\n", data);
	execve(path_cmd, cmd->argv, data->envp);
	error("execute error in child\n", data);
}

// TODO: exit with data->exit_code when bash send kill signal
// TODO: add error to system call failure
void	execute(t_msdata *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		wstatus;
	int		statuscode;
	int		org_shlvl;

	statuscode = -1;
	cmd = data->cmd_head;
	debugger("\n------------execution----------------\n\n");
	while (cmd)
	{
		if (cmd->pipe != NULL)
		{
			if (pipe(cmd->pipe->pipefd) == -1)
				error("pipe error\n", data);
		}
		if (cmd->pipe == NULL && cmd == data->cmd_head)
		{
			execute_child_dup_fd(data, cmd);
			statuscode = execute_check_builtin(data, cmd);
			execute_parent_close_pipe(data, cmd);
		}
		if (statuscode == -1)
		{
			execute_child_minishell(cmd);
			pid = fork();
			if (pid < 0)
				error("fork error\n", data);
			if (pid == 0)
				execute_child(data, cmd);
			execute_parent_close_pipe(data, cmd);
		}
		cmd = cmd->pipe;
	}
	while(waitpid(pid, &wstatus, 0) != -1 || errno != ECHILD);
	// if (WIFSIGNALED(wstatus))
	// 	statuscode = WEXITSTATUS(wstatus);
	// else
	if (WIFEXITED(wstatus))
		statuscode = WEXITSTATUS(wstatus);
	data->exit_code = statuscode;
	is_child = 1;
	return ;
}