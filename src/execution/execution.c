/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/02 12:51:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/04 17:21:23 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

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
		return (-1);
	if (cmd->infd > 0)
	{
		if (dup2(cmd->infd, STDIN_FILENO) == -1)
			error("dup error child infd to stdin");
		if (close(cmd->infd) == -1)
			error("close error child infd after dub to stdin");
		if (!(data->cmd_head == cmd))
		{
			if (close(cmd->pipefd[RD]) == -1)
				error("close error child read end pipe after fd dub to stdout");
		}
		ret = 1;
	}
	if (cmd->outfd > 0)
	{
		if (dup2(cmd->outfd, STDOUT_FILENO) == -1)
			error("dup error child outfd to stdout");
		if (close(cmd->outfd) == -1)
			error("close error child outfd after dub to stdout");
		if (cmd->pipe != NULL)
		{
			if (close(cmd->pipe->pipefd[WR]) == -1)
				error("close error child read end pipe after fd dub to stdin");
		}
		ret = 1;
	}
	return (ret);
}


void	execute_child_dup(t_msdata *data, t_cmd *cmd)
{
	int ret;

	ret = execute_child_dup_fd(data, cmd);
	if (ret == -1)
		exit (1);
	else if (ret == 1)
		return ; 
	if (cmd->pipe != NULL)
	{
		if (dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO) == -1)
			error("dup error child write end pipe to stdout");
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close error child write end pipe after dub to stdout");
		if (close(cmd->pipe->pipefd[RD]) == -1)
			error("close error child read end pipe after dub to stdout");
	}
	if (!(data->cmd_head == cmd))
	{
		if (dup2(cmd->pipefd[RD], STDIN_FILENO) == -1)
			error("dup error child read end pipe to stdin");
		if (close(cmd->pipefd[RD]) == -1)
			error("close error child read end pipe after dub to stdin");
	}
}

void	execute_parent_close_pipe(t_msdata *data, t_cmd *cmd)
{
	if (!(data->cmd_head == cmd))
	{
		if (close(cmd->pipefd[RD]) == -1)
			error("close error parent read end of pipe");
	}
	if (cmd->pipe != NULL)
	{
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close error parent write end of pipe");
	}
}

void	execute_child(t_msdata *data, t_cmd *cmd)
{
	char	*path_cmd;

	execute_child_dup(data, cmd);
	if (execute_path(cmd->cmd, data, &path_cmd) == -1)
		error("execute_child execute path error\n");
	if (add_command_to_argv(data, &cmd) == -1)
		error("add command to argv malloc error\n");
	execve(path_cmd, cmd->argv, data->envp);
	error("execute error in child\n");
}

void	execute(t_msdata *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		wstatus;
	int		statuscode;

	cmd = data->cmd_head;
	ft_printf("\n------------execution----------------\n\n");
	while (cmd)
	{
		if (cmd->pipe != NULL)
		{
			if (pipe(cmd->pipe->pipefd) == -1)
				error("pipe error\n");
		}
		pid = fork();
		if (pid < 0)
			error("fork error\n");
		if (pid == 0)
			execute_child(data, cmd);
		execute_parent_close_pipe(data, cmd);
		cmd = cmd->pipe;
	}
	while(waitpid(pid, &wstatus, 0) != -1 || errno != ECHILD);
	if (WIFEXITED(wstatus))
		statuscode = WEXITSTATUS(wstatus);
	//while loop to wait for child waitpid(?, ?,?) exit_code
	//WIFSIGNAL()
		//whatever it is returned += 128
	//WIFEXIT()
	return ;
}
