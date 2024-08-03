/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_wait.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/01 18:23:52 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 13:08:11 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sig;

int	execute_wait(int pid, int *wstatus, t_msdata *data)
{
	pid_t	result;

	result = waitpid(-1, wstatus, 0);
	if (errno == ECHILD)
		return (0);
	if (result == -1)
	{
		debugger(RED "Parent interruption in pid: %d\n" RESET, getpid());
		if (errno == EINTR)
		{
			if (g_sig == 3 && data->childs->next->pid == pid)
				kill(pid, SIGUSR1);
			return (1);
		}
		else
			return (0);
	}
	return (1);
}
