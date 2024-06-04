/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:46:31 by spenning          #+#    #+#             */
/*   Updated: 2024/06/04 13:51:09 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_signal(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sig == SIGINT)
		rl_on_new_line();
	if (sig == SIGQUIT)
		rl_on_new_line();
}

void	init_signal()
{
	struct sigaction	sa;
	int					ret;

	ret = 0;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	ret += sigaction(SIGINT, &sa, 0);
	ret += sigaction(SIGQUIT, &sa, 0);
	if (ret)
		exit(EXIT_FAILURE);
}
