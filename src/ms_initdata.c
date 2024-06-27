/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_initdata.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:50:51 by crasche           #+#    #+#             */
/*   Updated: 2024/06/27 14:29:55 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_initdata_cpy_envp(t_msdata *data, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	data->envp = ft_calloc(i + 1, sizeof(char *));
	if (!data->envp)
		ms_error("ms_initdata_envp, malloc error");
	data->envp[i] = NULL;
	i = 0;
	while (envp && envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		if (!data->envp[i])
			ms_error("ms_initdata_envp, malloc error");
		i++;
	}
}

void	ms_initdata(t_msdata *data, char **argv, char **envp)
{
	data->cmd_head = ft_calloc(sizeof(t_cmd), 1);
	data->cmd_curr = data->cmd_head;
	data->line = NULL;
	data->pos = 0;
	data->argv = argv;
	data->exp = NULL;
	ms_initdata_cpy_envp(data, envp);
}
