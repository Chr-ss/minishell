/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_handler.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/25 13:35:46 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/01 18:20:59 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	g_sig;

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
	(void)sig;

	g_sig = sig;
	if (sig == SIGUSR1)
		printf("\n");
}
