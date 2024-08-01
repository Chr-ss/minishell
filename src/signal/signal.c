/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/04 13:46:31 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/01 18:20:00 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// https://docs.rtems.org/releases/4.5.1-pre3/toolsdoc/
// gdb-5.0-docs/readline/readline00030.html

void	init_signal_interactive(struct sigaction *sa, t_msdata *data)
{
	int	ret;

	debugger("interactive\n");
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
			exit(EXIT_FAILURE);
	}
}

void	init_signal_execution(struct sigaction *sa, t_msdata *data)
{
	int	ret;

	debugger("exe\n");
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
			exit(EXIT_FAILURE);
	}
}

void	init_signal(t_msdata *data, bool execution)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (!execution)
		init_signal_interactive(&sa, data);
	else
		init_signal_execution(&sa, data);
}
