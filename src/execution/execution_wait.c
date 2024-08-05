/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_wait.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/01 18:23:52 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/05 17:26:23 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/execution.h"

extern int	g_sig;


int	execute_wait_minishell(t_cmd *cmd)
{
	int		len;

	len = ft_strlen(cmd->cmd);
	len -= 9;
	if (len < 0)
		return (0);
	if (!(ft_strncmp("minishell", cmd->cmd + len, 9)))
		return (1);
	return (0);
}

int	execute_wait(int pid, int *wstatus, t_msdata *data, int *statuscode)
{
	pid_t	result;

	result = waitpid(pid, wstatus, 0);
	execute_exit(wstatus, statuscode);
	if (errno == ECHILD)
		return (0);
	if (result == -1)
	{
		debugger(RED "Parent interruption in pid: %d\n" RESET, getpid());
		if (errno == EINTR)
		{
			if (g_sig == SIGINT && !(execute_wait_minishell(data->cmd_head)))
				printf("\n");
			if (g_sig == SIGQUIT && !(execute_wait_minishell(data->cmd_head)))
			{
				kill(pid, SIGUSR1);
				printf("\n");
			}
			return (1);
		}
		else
			return (0);
	}
	return (1);
}
