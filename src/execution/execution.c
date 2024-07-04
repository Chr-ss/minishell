/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/02 12:51:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/04 12:16:10 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

// REFERENCE: https://reactive.so/post/42-a-comprehensive-guide-to-pipex
// REFERENCE: https://www.gnu.org/software/libc/manual/html_node/
// Testing-File-Type.html
// REFERENCE: https://janelbrandon.medium.com/understanding-the-path-variable 
// -6eae0936e976

void	execute_child_dup(t_msdata *data, t_cmd *cmd)
{
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
	execute_path(cmd->cmd, data, &path_cmd);
	if(add_command_to_argv(data, &cmd) == -1)
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
			// ft_printf("pipefd[0] %d\n", cmd->pipe->pipefd[0]);
			// ft_printf("pipefd[1] %d\n", cmd->pipe->pipefd[1]);
		}
		pid = fork();
		if (pid < 0)
			error("fork error\n");
		if (pid == 0)
			execute_child(data, cmd);
		// ft_printf("parent\n");
		// ft_printf("cmd %s\n", cmd->cmd);
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
