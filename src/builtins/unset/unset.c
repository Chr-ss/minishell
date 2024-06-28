/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:20:04 by spenning          #+#    #+#             */
/*   Updated: 2024/06/28 15:19:32 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char **unset_new_envp(t_msdata *data, int skip_index)
{
	int		index;
	int		old_index;
	int		envp_len;
	char	**new_envp;

	index = 0;
	old_index = 0;
	envp_len = double_array_len(data->envp);
	new_envp = ft_calloc(envp_len, sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	new_envp[--envp_len] = NULL;
	while (data->envp[old_index] != NULL)
	{
		if (old_index == envp_len)
			break ;
		if (old_index == skip_index)
			old_index++;
		copy_over_str(index, old_index, new_envp, data->envp);
		index++;
		old_index++;
	}
	return (new_envp);
}

void unset(t_msdata *data, char	*arg)
{
	int		arglen;
	int		env_index;
	char	**new_envp;

	arglen = double_array_len(data->argv);
	if (arglen > 2)
		return ;
	if (arg == NULL)
		env_index = get_envp_index(arg, data->envp);
	else 
		env_index = get_envp_index(data->argv[1], data->envp);
	if (env_index == -1)
		return ;
	new_envp = unset_new_envp(data, env_index);
	if (new_envp == NULL)
		return ;
	swap_envp(data, new_envp);
}
