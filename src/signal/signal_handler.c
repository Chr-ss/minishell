/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_handler.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/25 13:35:46 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/25 13:47:32 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// extern bool	g_is_child;
extern pid_t	g_pid;

void	handle_signal(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signal_execution(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	debugger("send signal %d to %d\n", sig, g_pid);
	if (sig == SIGQUIT)
	{
		kill(g_pid, SIGINT);
		printf("\n");
	}
	if (sig == SIGINT)
	{
		kill(g_pid, SIGQUIT);
		printf("\n");
	}
}

void	handle_signal_minishell(int sig, siginfo_t *info, void *uc)
{
	(void)info;
	(void)uc;
	debugger("send signal %d to %d\n", sig, g_pid);
	if (sig == SIGINT)
		kill(g_pid, SIGQUIT);
}
