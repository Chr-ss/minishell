/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/23 14:39:27 by crasche       ########   odam.nl         */
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
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.");
		printf(">>>%s\n\n", data->line);
		temp_print_tokens(data, data->line);
		parsing(data);
		free(data->line);
		data->line = NULL;
	}
}
