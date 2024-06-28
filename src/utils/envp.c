/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:41:23 by spenning          #+#    #+#             */
/*   Updated: 2024/06/28 14:38:12 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	change_envp(char *key, char *env, char **envp)
{
	int		index;
	char	*temp;
	char	*concat;

	index = get_envp_index(key, envp);
	if (index == -1)
		return (EXIT_FAILURE);
	temp = ft_strjoin(key, "=");
	if (temp == NULL)
		return (EXIT_FAILURE);
	concat = ft_strjoin(temp, env);
	free(temp);
	if (concat == NULL)
		return (EXIT_FAILURE);
	temp = envp[index];
	envp[index] = concat;
	free(temp);
	return (EXIT_SUCCESS);
}

void	swap_envp(t_msdata *data, char **envp)
{
	free_char_array (data->envp);
	data->envp = envp;
}

