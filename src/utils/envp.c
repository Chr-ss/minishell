/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   envp.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 18:41:23 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/09 14:27:01 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	change_envp(char *key, char *env, t_msdata *data)
{
	int		index;
	char	*temp;
	char	*concat;

	if (!env)
		return (EXIT_FAILURE);
	index = get_envp_index(key, data->envp);
	if (index == -1)
		return (EXIT_FAILURE);
	temp = ft_strjoin(key, "=");
	if (temp == NULL)
		error("change_envp malloc error", data);
	concat = ft_strjoin(temp, env);
	free(temp);
	if (concat == NULL)
		error("change_envp malloc error", data);
	temp = data->envp[index];
	data->envp[index] = concat;
	free(temp);
	return (EXIT_SUCCESS);
}

void	swap_envp(t_msdata *data, char **envp)
{
	free_char_array (data->envp);
	data->envp = envp;
}
