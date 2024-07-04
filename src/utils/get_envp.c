/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_envp.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 14:34:53 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/04 15:39:35 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_envp_key(char *input)
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

char	*get_envp_value(char *input)
{
	int		start;
	int		end;
	int		len;
	char	*value;

	start = 0;
	end = 0;
	len = ft_strlen(input);
	while (input[start] != '=' && input[start] != '\0')
		start++;
	if (start == len)
	{
		value = ft_strdup("");
		if (value == NULL)
			return (NULL);
		return (value);
	}
	end = ++start;
	while (input[end] != '\0')
		end++;
	value = ft_calloc(((end - start) + 1), 1);
	if (value == NULL)
		return (NULL);
	ft_strlcpy(value, input + start, (end - start) + 1);
	return (value);
}

//MARK:add extra err?
int	get_envp_index(char *env, char **envp)
{
	int		index;
	char	*key;

	index = 0;
	while (envp[index] != NULL)
	{
		key = get_envp_key(envp[index]);
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

char	*get_envp_value_pointer(char *envp)
{
	char	*env;

	env = ft_strchr(envp, '=');
	env++;
	if (env == NULL)
		return (NULL);
	return (env);
}

int	get_envp(t_msdata *data, char *envp, char **env)
{
	int		index;

	*env = NULL;
	index = get_envp_index(envp, data->envp);
	if (index == -1)
		return (1);
	else
		*env = get_envp_value_pointer(data->envp[index]);
	if (*env != NULL)
	{
		*env = ft_strdup(*env);
		if (*env == NULL)
			return (-1);
	}
	else 
		return (1);
	return (0);
}
