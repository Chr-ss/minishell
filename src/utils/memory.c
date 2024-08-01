/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 18:43:01 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/01 16:27:38 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_all(t_msdata *data)
{
	free_cmd(data->cmd_head);
	free_data(data);
	rl_clear_history();
}

void	free_data(t_msdata *data)
{
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->envp)
	{
		free_char_array(data->envp);
		data->envp = NULL;
	}
	if (data->cmd_head)
		free(data->cmd_head);
	if (data->childs)
	{
		kill_all_childs(data);
		while (data->childs)
			delete_last_child(data);
	}
}

void	free_char_array(char **arr)
{
	int	index;

	index = 0;
	if (arr == NULL)
		return ;
	while (arr[index] != NULL)
	{
		free(arr[index]);
		arr[index] = 0;
		index++;
	}
	free(arr);
	arr = NULL;
}

int	copy_over_str(int dst_index, int src_index, char**dst, char **src)
{
	dst[dst_index] = ft_strdup(src[src_index]);
	if (!dst[dst_index])
	{
		free_char_array(dst);
		return (1);
	}
	return (0);
}
