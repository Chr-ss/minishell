/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_utils_chriss.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/01 15:01:31 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/01 19:09:24 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*str_expand_new(char *original_str, int *capacity)
{
	char	*new_str;

	*capacity *= 2;
	new_str = ft_calloc(*capacity + 1, sizeof(char));
	if(!new_str)
		return (NULL);
	if (original_str)
	{
		ft_strlcpy(new_str, original_str, ft_strlen(original_str) + 1);
		free(original_str);
	}
	return (new_str);
}
