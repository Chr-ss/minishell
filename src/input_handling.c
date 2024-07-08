/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/08 14:48:09 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int g_sigint ;

void	input_handling(t_msdata *data)
{
	while (1)
	{
		data->line = readline("minishell:~$");
		if (!data->line)
			error("Ok, EMPTY. EXIT");
		if (data->line[0])
			add_history(data->line);
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.");
	// EXPAND DEBUG //
	printf("\nexpanded:~$%s\n\n", data->line);
	// EXPAND DEBUG //
		parsing(data);
		temp_print_tokens(data, data->line);
		free(data->line);
		data->line = NULL;
		heredoc(data);
	// CMD PRINTING DEBUG //
	printf("\n");
	printf_cmd(data->cmd_head);
	// CMD PRINTING DEBUG //
		execute(data);
		clearcmd(data);
	}
}
