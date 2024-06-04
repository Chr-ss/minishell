/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:29:43 by crasche           #+#    #+#             */
/*   Updated: 2024/06/04 13:47:10 by spenning         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_msdata	data;
	char		*buffer;

	if (argc > 1)
		ms_error("Invalid argument count.");
	init_signal();
	(void) argv;
	ms_parsing(&data);
	buffer = ms_readline(&data);
	printf("MAIN: %s", buffer);
	ms_error("No, error.");
	return (0);
}
