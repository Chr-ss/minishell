/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/24 09:22:02 by mynodeus      ########   odam.nl         */
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
		data->line = readline("minishell:~$");
		debugger("errno %d\n", errno);
		if (!data->line)
			mini_exit(data, NULL, data->exit_code);
		if (data->line[0])
			add_history(data->line);
		debugger("errno %d\n", errno);
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.", data);
		debugger("\nexpanded:~$%s\n\n", data->line);
		debugger("errno %d\n", errno);
		if (parsing(data) == -1)
			continue ;
		debugger("errno %d\n", errno);
		if (line_to_token(data, data->line) == -1)
			continue ;
		debugger("errno %d\n", errno);
		if (heredoc(data) == -1)
			break ;
		debugger("errno %d\n", errno);
		debugger("\n");
		printf_cmd(data->cmd_head);
		debugger("errno %d\n", errno);
		execute(data);
		debugger("errno %d\n", errno);
		cmd_reset(data);
	}
}
