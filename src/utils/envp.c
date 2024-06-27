/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:41:23 by spenning          #+#    #+#             */
/*   Updated: 2024/06/27 16:29:07 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//MARK:Make util func
char	*get_key(char *input)
{
	int		index;
	char	*key;

	index = 0;
	while (input[index] != '\0')
	{
		if (input[index] == '=')
			break ;
		index++;
	}
	index++;
	key = ft_calloc(index, 1);
	if (key == NULL)
		return (NULL);
	ft_strlcpy(key, input, index);
	return (key);
}

//MARK:add extra err?
int	get_envp_index(char *env, char **envp)
{
	int		index;
	char	*key;

	index = 0;
	while (envp[index] != NULL)
	{
		key = get_key(envp[index]);
		if (key == NULL)
			return (-1);
		if (!ft_strncmp(env, key, ft_strlen(key)))
		{
			free(key);
			return (index);
		}
		free(key);
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

void	swap_envp(t_msdata *data, char **envp)
{
	free_char_array (data->envp);
	data->envp = envp;
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
