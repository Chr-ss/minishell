/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/02 12:51:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/25 13:32:40 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

// bool	g_is_child = 1;
pid_t	g_pid;

// REFERENCE: https://reactive.so/post/42-a-comprehensive-guide-to-pipex
// REFERENCE: https://www.gnu.org/software/libc/manual/html_node/
// Testing-File-Type.html
// REFERENCE: https://janelbrandon.medium.com/understanding-the-path-variable
// -6eae0936e976
// REFERENCE: https://stackoverflow.com/questions/47441871/why-should-we-check
// -wifexited-after-wait-in-order-to-kill-child-processes-in-lin
// REFERENCE: https://people.cs.rutgers.edu/~pxk/416/notes/c-tutorials/wait.html

void	execute_parent_restore_fds(t_msdata *data)
{
	if (data->org_stdin > 0)
	{
		if (dup2(data->org_stdin, STDIN_FILENO) == -1)
			error("dup org_stdout to stdout", data);
		if (close(data->org_stdin) == -1)
			error("close error org_stdout", data);
		data->org_stdin = -2;
	}
	if (data->org_stdout > 0)
	{
		if (dup2(data->org_stdout, STDOUT_FILENO) == -1)
			error("dup org_stdout to stdout", data);
		if (close(data->org_stdout) == -1)
			error("close error org_stdout", data);
		data->org_stdout = -2;
	}
}

void	execute_parent_close_pipe(t_msdata *data, t_cmd *cmd)
{
	if (cmd->pipe != NULL)
	{
		if (close(cmd->pipe->pipefd[WR]) == -1)
			error("close error parent write end of pipe", data);
	}
}

void	execute_pipe_child(t_msdata *data, t_cmd *cmd, int *g_pid)
{
	*g_pid = fork();
	if (*g_pid < 0)
		error("fork error\n", data);
	if (*g_pid == 0)
	{
		execute_child(data, cmd);
	}
}

void	execute_pipe(t_msdata *data, t_cmd *cmd, int *g_pid, int *statuscode)
{
	if (cmd->pipe != NULL)
	{
		if (pipe(cmd->pipe->pipefd) == -1)
			error("pipe error\n", data);
	}
	if (execute_child_dup(data, cmd))
	{
		data->overrule_exit = true;
		execute_parent_close_pipe(data, cmd);
		execute_parent_restore_fds(data);
		return ;
	}
	if (cmd->pipe == NULL && cmd == data->cmd_head)
		*statuscode = execute_check_builtin(data, cmd);
	if (cmd->pipe == NULL && *statuscode)
		data->overrule_exit = false;
	if (*statuscode == -1)
		execute_pipe_child(data, cmd, g_pid);
	init_signal(data, true, false);
	execute_child_minishell(data, cmd);
	execute_parent_close_pipe(data, cmd);
	execute_parent_restore_fds(data);
}

// TODO: exit with data->exit_code when bash send kill signal
// TODO: add error to system call failure
// TODO: check if wifsignal should still be added
void	execute(t_msdata *data)
{
	t_cmd	*cmd;
	int		wstatus;
	int		statuscode;

	wstatus = -1;
	statuscode = -1;
	cmd = data->cmd_head;
	while (cmd)
	{
		if (cmd->cmd)
			execute_pipe(data, cmd, &g_pid, &statuscode);
		else
			statuscode = 1;
		cmd = cmd->pipe;
	}
	while (waitpid(-1, &wstatus, 0) != -1 || errno != ECHILD)
		;
	init_signal(data, false, false);
	if (WIFEXITED(wstatus) || WIFSTOPPED(wstatus))
		statuscode = WEXITSTATUS(wstatus);
	if (data->overrule_exit == true)
		statuscode = 1;
	data->exit_code = statuscode;
}
