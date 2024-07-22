/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_print.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 13:51:40 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/22 14:11:19 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/**
 * @brief This is a function to sort the envp for export
 * @param data 
 * @param index 
 * @param inner_index 
 * @return void 
 */
void	export_pes(t_msdata *data, int index, int inner_index)
{
	int		len;
	int		inner_len;
	char	*temp;

	len = 0;
	inner_len = 0;
	len = ft_strlen(data->envp[index]);
	inner_len = ft_strlen(data->envp[inner_index]);
	if (inner_len > len)
		len = inner_len;
	if (ft_strncmp(data->envp[inner_index], data->envp[index], len) > 0)
	{
		temp = data->envp[index];
		data->envp[index] = data->envp[inner_index];
		data->envp[inner_index] = temp;
	}
}

int	export_print_env(t_msdata *data)
{
	int		index;
	int		inner_index;

	index = 0;
	inner_index = 1;
	while (data->envp[index] != NULL)
	{
		while (data->envp[inner_index] != NULL)
		{
			export_pes(data, index, inner_index);
			inner_index++;
		}
		inner_index = 0;
		index++;
	}
	index = 0;
	while (data->envp[index] != NULL)
		ft_printf("declare -x %s\n", data->envp[index++]);
	return (0);
}