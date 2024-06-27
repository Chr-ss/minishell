/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:02:08 by spenning          #+#    #+#             */
/*   Updated: 2024/06/27 15:11:40 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_envp_swap(t_msdata *data, char **envp)
{
	free_char_array (data->envp);
	data->envp = envp;
}

char	*add_envp_new_entry(char **new_envp, char *key, char *value)
{
	char	*new_entry;
	char	*temp;

	temp = ft_strjoin(key, "=");
	if (temp == NULL)
	{
		free_char_array(new_envp);
		return (NULL);
	}
	new_entry = ft_strjoin(temp, value);
	free(temp);
	if (new_entry == NULL)
	{
		free_char_array(new_envp);
		return (NULL);
	}
	return (new_entry);
}

void	add_envp(t_msdata *data, char *key, char *value)
{
	char	**new_envp;
	int		len;
	int		index;
	char	*new_entry;

	index = 0;
	len = double_array_len(data->envp) + 1;
	new_envp = ft_calloc(len + 1, sizeof(char *));
	if (new_envp == NULL)
		ms_error("add_envp, envp double char array malloc error");
	new_envp[len] = NULL;
	while (data->envp[index] != NULL)
	{
		new_envp[index] = ft_strdup(data->envp[index]);
		if (!new_envp[index])
			ms_error("add_envp, copying envp malloc error");
		index++;
	}
	new_entry = add_envp_new_entry(new_envp, key, value);
	if (new_entry == NULL)
		ms_error("add_envp, new entry join malloc error #1");
	new_envp[index] = new_entry;
	add_envp_swap(data, new_envp);
}
