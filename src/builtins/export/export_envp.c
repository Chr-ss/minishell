/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export_envp.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/08 21:02:38 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/20 17:58:54 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	export_envp(t_msdata *data, char *input, char *key, char *value)
{
	int	check_envp;

	check_envp = get_envp_index(key, data->envp);
	if (check_envp != -1 && ft_strchr(input, '='))
		unset(data, NULL, key);
	if (check_envp == -1)
		add_envp(data, key, value);
	else if (check_envp == -1 && !ft_strchr(input, '='))
		add_envp(data, key, value);
	else if (check_envp != -1 && ft_strchr(input, '='))
		add_envp(data, key, value);
}
