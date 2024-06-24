/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:41:23 by spenning          #+#    #+#             */
/*   Updated: 2024/06/24 18:58:47 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_envp_index(char *env, char **envp)
{
	int	index;

	index = 0;
	while (envp[index] != NULL)
	{
		if (!ft_strncmp(env, envp[index], ft_strlen(env)))
			return (index);
		index++;
	}
	return (-1);
}

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

char	*get_envp_value(char *envp)
{
	char	*env;

	env = ft_strchr(envp, '=');
	env++;
	if (env == NULL)
		return (NULL);
	return (env);
}

//TODO change to return int, and have pointer 
// as parameter
char	*get_envp(t_msdata *data, char *envp)
{
	int		index;
	char	*path;

	path = NULL;
	index = get_envp_index(envp, data->envp);
	if (index == -1)
		ft_printf(" %s not set\n", envp);
	else
		path = get_envp_value(data->envp[index]);
	if (path != NULL)
	{
		path = ft_strdup(path);
		if (path == NULL)
			return (NULL);
	}
	return (path);
}
