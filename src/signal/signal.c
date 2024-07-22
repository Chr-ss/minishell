/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/04 13:46:31 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/22 14:28:20 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern bool	g_is_child;

// https://docs.rtems.org/releases/4.5.1-pre3/toolsdoc/
// gdb-5.0-docs/readline/readline00030.html

void	handle_signal(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sig == SIGINT && g_is_child)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

//TODO: check if the exit here should be changed to mini_exit or no

void	init_signal(void)
{
	struct sigaction	sa;
	int					ret;

	ret = 0;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	signal(SIGQUIT, SIG_IGN);
	ret += sigaction(SIGINT, &sa, 0);
	if (ret)
		exit(EXIT_FAILURE);
}
