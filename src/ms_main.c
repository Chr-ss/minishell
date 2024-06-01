/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_main.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/01 18:12:41 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_error(char *msg)
{
	perror(msg);
	exit(1);
}

void	ms_init_cmdlist(t_cmd **cmd)
{
	*cmd = ft_calloc(sizeof(t_cmd), 1);
}

void	ms_parsing(t_msdata *data)
{
	ms_init_cmdlist(&data->cmd);
}

void	handle_signal(int sig, siginfo_t *info, void *ucontext)
{
	(void)info;
	(void)ucontext;
	if (sig == SIGINT)
		rl_on_new_line();
	if (sig == SIGQUIT)
		rl_on_new_line();
}

void	ms_init_signal()
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

int	main(int argc, char **argv)
{
	t_msdata	data;
	char		*buffer;

	if (argc > 1)
		ms_error("Invalid argument count.");
	ms_init_signal();
	(void) argv;
	ms_parsing(&data);
	buffer = ms_readline(&data);
	printf("MAIN: %s", buffer);
	ms_error("No, error.");
	return (0);
}


// TEST //

// TEST //

// TEST //

// TEST //