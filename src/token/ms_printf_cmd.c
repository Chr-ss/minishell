/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_printf_cmd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:05:57 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/12 16:06:26 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	printf_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	printf("\n\t ## CMD ## \n");
	while (cmd)
	{
		if (cmd->cmd)
			printf("CMD:%s\n", cmd->cmd);
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			printf("ARGV[%d]:%s\n", i, cmd->argv[i]);
			i++;
		}
		if (cmd->in)
			printf("INFILE:%s\n", cmd->in);
		i = 0;
		while (cmd->out && cmd->out[i])
		{
			printf("OUT[%d]:%s\n", i, cmd->out[i]);
			i++;
		}
		if (cmd->append)
			printf("APPEND:%d\n", cmd->append);
		cmd = cmd->pipe;
	}
	printf("\n\n");
}

