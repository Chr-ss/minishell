/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_utils_chriss.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/01 15:01:31 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/23 14:28:21 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_skipspace(char *str, int pos)
{
	while (str && str[pos] && ft_isspace(str[pos]))
		pos++;
	return (pos);
}

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