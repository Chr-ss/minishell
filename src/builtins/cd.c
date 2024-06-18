
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:41:51 by spenning          #+#    #+#             */
/*   Updated: 2024/06/18 15:06:51 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int get_envp_index(char *env, char **envp)
{
	int index;

	index = 0;

	while (envp[index] != NULL)
	{
		if (ft_strncmp(env, envp[index], ft_strlen(env)))
			return (index);
		index++;
	}
	return (-1);
}

int cd (t_msdata *data)
{
	for (int i = 1; data->argv[i] != NULL; i++)
	{
		ft_printf("arg: %s\n", data->argv[i]);
	}
	for (int i = 0; data->envp[i] != NULL; i++)
	{
		ft_printf("env: %s\n", data->envp[i]);
	}
	ft_printf("HOME index %d\n", get_envp_index("HOME", data->envp));
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
