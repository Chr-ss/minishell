/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:20:04 by spenning          #+#    #+#             */
/*   Updated: 2024/06/27 16:27:52 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

//MARK:fix segfault
char **unset_new_envp(t_msdata *data, int skip_index)
{
	int		index;
	int		envp_len;
	char	**new_envp;

	index = 0;
	envp_len = double_array_len(data->envp);
	new_envp = ft_calloc(envp_len, sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	new_envp[--envp_len] = NULL;
	ft_printf("skip_index %d\n", skip_index);
	while (data->envp[index] != NULL)
	{
		if (index == skip_index)
			index++;
		copy_over_str(index, new_envp, data->envp);
		ft_printf("index %d %s %s\n", index, new_envp[index], data->envp[index]);
		index++;
	}
	return (new_envp);
}

void unset(t_msdata *data)
{
	int		arglen;
	int		env_index;
	char	**new_envp;
	// int		index;

	arglen = double_array_len(data->argv);
	if (arglen > 2)
		return ;
	env_index = get_envp_index(data->argv[1], data->envp);
	if (env_index == -1)
		return ;
	new_envp = unset_new_envp(data, env_index);
	if (new_envp == NULL)
		return ;
	swap_envp(data, new_envp);
	// index = 0;
	// while (data->envp[index] != NULL)
	// {
	// 	ft_printf("index %d %s\n", index, data->envp[index]);
	// 	index++;
	// }
}
