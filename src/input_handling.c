/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/01 18:49:28 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	input_handling(t_msdata *data)
{
	while (1)
	{
		if (data->line)
			free(data->line);
		data->line = NULL;
		data->line = readline("minishell:~$ ");
		if (!data->line)
			mini_exit(data, NULL, data->exit_code);
		if (data->line[0])
			add_history(data->line);
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.", data);
		if (parsing(data) == -1)
			continue ;
		if (line_to_token(data, data->line) == -1)
			continue ;
		if (heredoc(data) == -1)
			break ;
		printf_cmd(data->cmd_head);
		execute(data);
		reset_childs(data);
		cmd_reset(data);
	}
}
