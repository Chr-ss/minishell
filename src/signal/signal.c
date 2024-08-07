/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/04 13:46:31 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/07 17:14:29 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// https://docs.rtems.org/releases/4.5.1-pre3/toolsdoc/
// gdb-5.0-docs/readline/readline00030.html

void	init_signal_interactive(struct sigaction *sa, t_msdata *data)
{
	int	ret;

	debugger(BMAG "Parent: Interactive mode\n" RESET);
	ret = 0;
	sa->sa_sigaction = handle_signal;
	signal(SIGQUIT, SIG_IGN);
	ret += sigaction(SIGINT, sa, 0);
	ret += sigaction(SIGUSR1, sa, 0);
	if (ret)
	{
		if (data)
			error("signal handler error", data);
		else
		{
			free_all(data);
			exit(EXIT_FAILURE);
		}
	}
}

void	init_signal_execution(struct sigaction *sa, t_msdata *data, bool *exec)
{
	int	ret;

	debugger(BMAG "Parent: execution mode\n" RESET);
	*exec = true;
	ret = 0;
	sa->sa_sigaction = handle_signal_execution;
	ret += sigaction(SIGINT, sa, 0);
	ret += sigaction(SIGQUIT, sa, 0);
	ret += sigaction(SIGUSR1, sa, 0);
	if (ret)
	{
		if (data)
			error("signal handler error", data);
		else
		{
			free_all(data);
			exit(EXIT_FAILURE);
		}
	}
}

void	init_signal_heredoc(struct sigaction *sa, t_msdata *data)
{
	int	ret;

	debugger(BMAG "Parent: heredoc mode\n" RESET);
	ret = 0;
	sa->sa_sigaction = handle_signal_heredoc;
	ret += sigaction(SIGINT, sa, 0);
	ret += sigaction(SIGQUIT, sa, 0);
	if (ret)
	{
		if (data)
			error("signal handler error", data);
		else
		{
			free_all(data);
			exit(EXIT_FAILURE);
		}
	}
}

void	init_signal_after_heredoc(struct sigaction *sa, \
		t_msdata *data, bool *exec)
{
	int	ret;

	debugger(BMAG "Parent: after heredoc mode\n" RESET);
	*exec = false;
	ret = 0;
	sa->sa_sigaction = handle_signal_after_heredoc;
	ret += sigaction(SIGINT, sa, 0);
	ret += sigaction(SIGQUIT, sa, 0);
	if (ret)
	{
		if (data)
			error("signal handler error", data);
		else
		{
			free_all(data);
			exit(EXIT_FAILURE);
		}
	}
}

void	init_signal(t_msdata *data, int type)
{
	struct sigaction	sa;
	static bool			ahd;
	static bool			exec;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (ahd == true && exec == true)
	{
		init_signal_after_heredoc(&sa, data, &exec);
		ahd = false;
		return ;
	}
	if (type == interactive)
		init_signal_interactive(&sa, data);
	else if (type == execution)
		init_signal_execution(&sa, data, &exec);
	else if (type == hd)
		init_signal_heredoc(&sa, data);
	else if (type == afterhd)
	{
		ahd = true;
		init_signal_after_heredoc(&sa, data, &exec);
	}
}
