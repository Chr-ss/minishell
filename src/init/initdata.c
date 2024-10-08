/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initdata.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:50:51 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/20 18:00:53 by spenning      ########   odam.nl         */
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

void	initdata_shlvl(t_msdata *data, int index)
{
	char	*shlvl;

	shlvl = get_envp_value(data->envp[index]);
	if (!shlvl)
		error("initdata: malloc error", data);
	index = ft_atoi(shlvl);
	free(shlvl);
	shlvl = ft_itoa(++index);
	if (!shlvl)
		error("initdata: malloc error", data);
	if (change_envp("SHLVL", shlvl, data))
		perror("SHLVL change error");
	free(shlvl);
}

void	initdata_pwd(t_msdata *data)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	pwd = NULL;
	pwd = get_envp_value("PWD");
	if (pwd == NULL)
		error("initdata PWD malloc error", data);
	if (!ft_strncmp(pwd, "", ft_strlen(pwd)))
		free (pwd);
	else
	{
		data->pwd = pwd;
		return ;
	}
	if (!getcwd(cwd, PATH_MAX))
	{
		free_all(data);
		perror("getcwd() failed");
		exit(0);
	}
	data->pwd = ft_strdup(cwd);
	if (data->pwd == NULL)
		error("cwd malloc fail", data);
}

void	initdata(t_msdata *data, char **envp)
{
	int		index;

	index = 0;
	data->cmd_head = ft_calloc(sizeof(t_cmd), 1);
	if (!data->cmd_head)
		error("initdata: malloc error", data);
	init_kindergarten(data);
	data->cmd_curr = data->cmd_head;
	data->line = NULL;
	data->pos = 0;
	data->exp = NULL;
	data->exit_code = 0;
	data->org_stdout = -2;
	data->org_stdin = -2;
	data->overrule_exit = false;
	initdata_cpy_envp(data, envp);
	index = get_envp_index("SHLVL", data->envp);
	if (index != -1)
		initdata_shlvl(data, index);
	initdata_pwd(data);
}
