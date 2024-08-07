/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_print.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 13:51:40 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/07 19:17:01 by spenning      ########   odam.nl         */
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

void	export_print_env_quotes(t_msdata *data, int index)
{
	char	*key;
	char	*value;

	key = get_envp_key(data->envp[index]);
	value = get_envp_value(data->envp[index]);
	if (key == NULL || value == NULL)
		error("export_print_env_quotes malloc err", data);
	if (ft_strchr(data->envp[index], '='))
		ft_printf("declare -x %s=\"%s\"\n", key, value);
	else
		ft_printf("declare -x %s %s\n", key, value);
	free(key);
	free(value);
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
		export_print_env_quotes(data, index++);
	return (0);
}
