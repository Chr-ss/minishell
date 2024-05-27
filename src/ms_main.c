/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_main.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/27 17:26:04 by veno          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_error(char *msg)
{
	perror(msg);
	exit(1);
}

void	ms_init_cmdlist(t_cmd *cmd)
{
	cmd = ft_calloc(sizeof(t_cmd), 1);
}

void	ms_parsing(t_msdata *data)
{
	ms_init_cmdlist(data->cmd);
}

int	main(int argc, char **argv)
{
	t_msdata	data;
	char		*buffer;

	if (argc > 1)
		ms_error("Invalid argument count.");
	(void) argv;
	ms_parsing(&data);
	buffer = ms_readline(&data);
	ms_error("No, error.");
	return (0);
}
