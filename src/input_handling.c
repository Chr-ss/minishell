/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 16:12:59 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_sigint ;

void	input_handling(t_msdata *data)
{
	while (1)
	{
		if (data->line)
			free(data->line);
		data->line = NULL;
		data->line = readline("minishell:~$");
		if (!data->line)
			exit(0);
		if (data->line[0])
			add_history(data->line);
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.", data);
// EXPAND DEBUG //
debugger("\nexpanded:~$%s\n\n", data->line);
// EXPAND DEBUG //
		if (parsing(data) == -1)
			break ;
		if (line_to_token(data, data->line) == -1)
			break ;
		if (heredoc(data) == -1)
			break ;
// CMD PRINTING DEBUG //
debugger("\n");
printf_cmd(data->cmd_head);
// CMD PRINTING DEBUG //
		execute(data);
		cmd_reset(data);
	}
}
