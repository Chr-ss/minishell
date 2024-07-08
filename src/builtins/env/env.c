/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:43:42 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/05 18:40:26 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	env(t_msdata *data)
{
	int	index;
	int	arglen;

	arglen = double_array_len(data->argv);
	if (arglen > 1)
		return 1;
	index = 0;
	while (data->envp[index] != NULL)
	{
		ft_printf("%s\n", data->envp[index]);
		index++;
	}
	return (0);
}
