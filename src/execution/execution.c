/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/02 12:51:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/23 13:49:42 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

bool	g_is_child = 1;

// REFERENCE: https://reactive.so/post/42-a-comprehensive-guide-to-pipex
// REFERENCE: https://www.gnu.org/software/libc/manual/html_node/
// Testing-File-Type.html
// REFERENCE: https://janelbrandon.medium.com/understanding-the-path-variable
// -6eae0936e976
// REFERENCE: https://stackoverflow.com/questions/47441871/why-should-we-check
// -wifexited-after-wait-in-order-to-kill-child-processes-in-lin
// REFERENCE: https://people.cs.rutgers.edu/~pxk/416/notes/c-tutorials/wait.html

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

void	execute_pipe(t_msdata *data, t_cmd *cmd, int *pid, int *statuscode)
{
	if (cmd->pipe != NULL)
	{
		if (pipe(cmd->pipe->pipefd) == -1)
			error("pipe error\n", data);
	}
	if (cmd->pipe == NULL && cmd == data->cmd_head)
	{
		if (execute_child_dup_fd(data, cmd) == -1)
		{
			*statuscode = 1;
			return ;
		}
		*statuscode = execute_check_builtin(data, cmd);
		execute_parent_close_pipe(data, cmd);
	}
	if (*statuscode == -1)
	{
		execute_child_minishell(cmd);
		*pid = fork();
		if (*pid < 0)
			error("fork error\n", data);
		if (*pid == 0)
			execute_child(data, cmd);
		execute_parent_close_pipe(data, cmd);
	}
}

// TODO: exit with data->exit_code when bash send kill signal
// TODO: add error to system call failure
// TODO: check if wifsignal should still be added
void	execute(t_msdata *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		wstatus;
	int		statuscode;

	pid = -1;
	wstatus = -1;
	statuscode = -1;
	cmd = data->cmd_head;
	debugger("\n------------execution----------------\n\n");
	while (cmd)
	{
		execute_pipe(data, cmd, &pid, &statuscode);
		cmd = cmd->pipe;
	}
	while (waitpid(pid, &wstatus, 0) != -1 || errno != ECHILD)
		;
	if (WIFEXITED(wstatus) || WIFSTOPPED(wstatus))
		statuscode = WEXITSTATUS(wstatus);
	data->exit_code = statuscode;
	g_is_child = 1;
	return ;
}
