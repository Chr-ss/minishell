/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/02 12:51:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/06 17:38:50 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

extern int	g_sig;

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
		if (cmd->pipefd[WR])
		{
			debugger (RED "Parent: closing cmd->pipefd[WR] %d\n" RESET, \
				cmd->pipefd[WR]);
		}
		if (cmd->pipefd[WR] && close(cmd->pipefd[WR]) == -1)
			error("close error parent cmd->pipefd[WR]", data);
		if (cmd->pipefd[RD])
		{
			debugger (RED "Parent: closing cmd->pipefd[RD] %d\n" RESET, \
				cmd->pipefd[RD]);
		}
		if (cmd->pipefd[RD] && close(cmd->pipefd[RD]) == -1)
			error("close error parent cmd->pipefd[RD]", data);
	}
}

void	execute_pipe_child(t_msdata *data, t_cmd *cmd, int *pid)
{
	*pid = fork();
	if (*pid < 0)
		error("fork error", data);
	if (*pid == 0)
	{
		if (execute_child_dup(data, cmd))
			mini_exit (data, NULL, 1);
		execute_child(data, cmd);
	}
	else
	{
		execute_parent_close_pipe(data, cmd);
		add_child(*pid, data);
	}
}

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

void	execute_pipe(t_msdata *data, t_cmd *cmd, int *pid, int *statuscode)
{
	debugger(BMAG "cmd: %s\n" RESET, cmd->cmd);
	if (cmd->pipe != NULL)
	{
		if (pipe(cmd->pipe->pipefd) == -1)
			error("pipe error", data);
	}
	if (cmd->pipe == NULL && cmd == data->cmd_head)
	{
		*statuscode = execute_check_builtin(data, cmd);
		execute_parent_restore_fds(data);
	}
	if (*statuscode == -1)
		execute_pipe_child(data, cmd, pid);
	init_signal(data, execution);
}

void	execute(t_msdata *data)
{
	t_cmd		*cmd;
	int			wstatus;
	int			statuscode;
	int			pid;
	t_childs	*last;

	last = NULL;
	pid = 0;
	wstatus = -1;
	statuscode = -1;
	cmd = data->cmd_head;
	while (cmd)
	{
		if (cmd->cmd)
			execute_pipe(data, cmd, &pid, &statuscode);
		else
			statuscode = 1;
		cmd = cmd->pipe;
	}
	last = get_last_child(data);
	while (execute_wait(last->pid, &wstatus, data, &statuscode))
		;
	execute_wait_all_children(&wstatus);
	init_signal(data, interactive);
	data->exit_code = statuscode;
}
