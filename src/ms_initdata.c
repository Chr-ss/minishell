/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_initdata.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:50:51 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/04 23:27:38 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_initdata_cpy_envp(t_msdata *data, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	data->envp = ft_calloc(i, sizeof(char *));
	if (!data->envp)
		ms_error("ms_initdata_envp, malloc error");
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
	data->cmd = ft_calloc(sizeof(t_cmd), 1);
	data->argv = argv;
	data->line = NULL;
	data->exp = NULL;
	ms_initdata_cpy_envp(data, envp);
}
