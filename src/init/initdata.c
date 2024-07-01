/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initdata.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:50:51 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/04 23:27:38 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	initdata_cpy_envp(t_msdata *data, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	data->envp = ft_calloc(i + 1, sizeof(char *));
	if (!data->envp)
		error("initdata_envp, malloc error");
	i = 0;
	while (envp && envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		if (!data->envp[i])
			error("initdata_envp, malloc error");
		i++;
	}
}

void	initdata(t_msdata *data, char **argv, char **envp)
{
	data->cmd_head = ft_calloc(sizeof(t_cmd), 1);
	data->cmd_curr = data->cmd_head;
	data->line = NULL;
	data->pos = 0;
	data->argv = argv;
	data->exp = NULL;
	initdata_cpy_envp(data, envp);
}
