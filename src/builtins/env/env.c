/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:43:42 by spenning          #+#    #+#             */
/*   Updated: 2024/06/25 14:50:41 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	env(t_msdata *data)
{
	int	index;
	int	arglen;

	arglen = double_array_len(data->argv);
	if (arglen > 1)
		return ;
	index = 0;
	while (data->envp[index] != NULL)
	{
		ft_printf("%s\n", data->envp[index]);
		index++;
	}
}
