/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/01 15:01:31 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 16:54:31 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	skipspace(char *str, int pos)
{
	while (str && str[pos] && ft_isspace(str[pos]))
		pos++;
	return (pos);
}

int	ft_isbashtoken(int c)
{
	if (c == '|' || c == '>' || c == '<' || c == 0)
		return (1);
	return (0);
}

char	**extend_strarr(char **strarr, int strarr_size)
{
	int		i;
	char	**new_strarr;

	i = 0;
	strarr_size++;
	new_strarr = ft_calloc(strarr_size + 1, sizeof(char*));
	if (!new_strarr)
	{
		if (strarr)
			free(strarr);
		return (NULL);
	}
	while (strarr && strarr[i])
	{
		new_strarr[i] = strarr[i];
		strarr[i] = NULL;
		i++;
	}
	if (strarr)
		free(strarr);
	return (new_strarr);
}