/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_exit.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/02 14:41:59 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 18:57:24 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sig;

void	execute_exit(int wstatus, int *statuscode)
{
	if (WIFEXITED(wstatus) || WIFSTOPPED(wstatus))
		*statuscode = WEXITSTATUS(wstatus);
	if (WIFSIGNALED(wstatus))
		*statuscode = (g_sig + 128);
}
