/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_exit.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/02 14:41:59 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/20 17:59:31 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sig;

void	execute_exit(int *wstatus, int *statuscode)
{
	if (WIFEXITED(*wstatus) || WIFSTOPPED(*wstatus))
		*statuscode = WEXITSTATUS(*wstatus);
	if (WIFSIGNALED(*wstatus))
	{
		*statuscode = (g_sig + 127);
		if (WTERMSIG(*wstatus) == 13)
			*statuscode = 0;
	}
}

void	execute_path_local_exit(void)
{
	if (errno == 2)
		exit(127);
	else if (errno == 20)
		exit(126);
}
