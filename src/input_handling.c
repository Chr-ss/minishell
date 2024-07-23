/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/23 18:51:40 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern bool	g_is_child;

void	input_handling(t_msdata *data)
{
	while (1)
	{
		if (data->line)
			free(data->line);
		data->line = NULL;
		debugger("here again\n\n");
		data->line = readline("minishell:~$");
		debugger("here again 2\n\n");
		if (!data->line)
			mini_exit(data, NULL, data->exit_code);
		if (data->line[0])
			add_history(data->line);
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.", data);
		debugger("\nexpanded:~$%s\n\n", data->line);
		if (parsing(data) == -1)
			continue ;
		if (line_to_token(data, data->line) == -1)
			continue ;
		if (heredoc(data) == -1)
			break ;
		debugger("\n");
		printf_cmd(data->cmd_head);
		execute(data);
		cmd_reset(data);
		debugger("\nafter execution\n\n");
	}
}
