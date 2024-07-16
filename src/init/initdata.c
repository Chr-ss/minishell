/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initdata.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:50:51 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/16 16:51:33 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	initdata_cpy_envp(t_msdata *data, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	data->envp = ft_calloc(i + 1, sizeof(char *));
	if (!data->envp)
		error("initdata_envp: malloc error", data);
	i = 0;
	while (envp && envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		if (!data->envp[i])
			error("initdata_envp: malloc error", data);
		i++;
	}
}

void	initdata(t_msdata *data, char **envp)
{
	data->cmd_head = ft_calloc(sizeof(t_cmd), 1);
	if (!data->cmd_head)
		error("initdata: malloc error", data);
	data->cmd_curr = data->cmd_head;
	data->line = NULL;
	data->pos = 0;
	data->exp = NULL;
	data->exit_code = 0;
	data->org_stdout = 0;
	initdata_cpy_envp(data, envp);
}
