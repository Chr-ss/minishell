/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_readline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/13 14:23:00 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int g_sigint ;

int	ms_strarr_size(char **strarr)
{
	int	i;

	i = 0;
	while (strarr && strarr[i])
		i++;
	return (i);
}

char	**ms_extend_strarr(t_cmd *cmd, char **strarr, int strarr_size)
{
	(void) cmd;
	int		i;
	char	**new_strarr;

	i = 0;
	strarr_size++;
	new_strarr = calloc(strarr_size + 1, sizeof(char*));
	if (!new_strarr)
		ms_error("ms_extend_strarr: malloc error.");
	while (strarr && strarr[i])
	{
		new_strarr[i] = strarr[i];
		strarr[i] = NULL;
		i++;
	}
	free(strarr);
	return (new_strarr);
}


// void	skipspace(char **string)
// {
// 	while (ft_isspace((int) **string))
// 		(*string)++;
// }


void	ms_readline(t_msdata *data)
{
	while (1)
	{
		data->line = readline("minishell:~$");
		if (!data->line)
			ms_error("Ok, EMPTY. EXIT");
		data->line = ms_expand(data);
		if (!data->line)
			ms_error("readline malloc error.");
		printf(">>>%s\n\n", data->line);
		temp_print_tokens(data, data->line);
		ms_parsing(data);
		free(data->line);
		data->line = NULL;
	}
}
