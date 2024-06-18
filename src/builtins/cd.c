
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

//https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory-exists
//https://stackoverflow.com/questions/291828/what-is-the-best-way-to-return-an-error-from-a-function-when-im-already-returni

#include "../../include/minishell.h"
#include <dirent.h>
#include <errno.h>

int get_envp_index(char *env, char **envp)
{
	int index;

	index = 0;
	while (envp[index] != NULL)
	{
		if (!ft_strncmp(env, envp[index], ft_strlen(env)))
			return (index);
		index++;
	}
	return (-1);
}

char	*get_envp_value(char *envp)
{
	char	*env;

	env = ft_strchr(envp, '=');
	env++;
	if (env == NULL)
		return (NULL);
	return (env);
}

int double_array_len(char **vector)
{
	int index;

	index = 0;
	while(vector[index] != NULL)
		index++;
	return (index);
}

// test index -1 // and implement fprintf instead of perror
char *get_envp(t_msdata *data, char *envp)
{
	int		index;
	char	*path;

	path = NULL;
	index = get_envp_index(envp, data->envp);
	if (index == -1)
		perror(" cd: HOME not set");
	else
		path = get_envp_value(data->envp[index]);
	return (path);
}

//psd == dot + slash + operand
char *cd_parse_cdpath_dso(char *cdpath, char *operand);

//psd == pathname + slash + operand
char *cd_parse_cdpath_pso(char *cdpath, char *operand)
{
	char	**sp_cdpath;
	int		index;
	DIR*	dir;
	char	*dirname;

	index = 0;
	sp_cdpath = ft_split(cdpath, ':');
	if (sp_cdpath == NULL)
		return (NULL);
	while (sp_cdpath[index] != NULL)
	{
		if (sp_cdpath[index][ft_strlen(sp_cdpath[index]) - 1] != '/')
		{
			dirname = ft_strjoin(sp_cdpath[index], "/");
			dirname = ft_strjoin(dir, operand);
		}
		if (!dirname)
			return (NULL);
		dir = opendir(dirname);
		if (dir) 
		{
			closedir(dir);
			return (dirname);
		}
		else if (ENOENT == errno) 
			index ++;
		else 
			return (NULL);
	}
}

char *cd_parse(t_msdata *data)
{
	char operand[2];
	char *cdpath;

	cdpath = NULL;
	ft_strlcpy(operand, data->argv[1], 2);
	if (operand[0] == '/')
		return (data->argv[1]);
	else if (operand[0] == '.' || !ft_strncmp(operand, "..", 2))
		return (data->argv[1]);
	else
	{
		cdpath = cd_parse_cdpath_pso(get_envp(data, "CDPATH"), data->argv[1]);
		if (cdpath == NULL)
			cdpath = cd_parse_cdpath_dso(get_envp(data, "CDPATH"), data->argv[1]);
		return (cdpath);
	}
}
//TODO test .:~/projects/core_projects/minishell/src with a directory at that path location while calling cd in another directory

// change dir to be malloced always
int cd (t_msdata *data)
{
	char * dir;
	// char cwd[PATH_MAX];

	dir = NULL;
	ft_printf("%d\n", double_array_len(data->argv));
	if (double_array_len(data->argv) == 1)
		dir = get_envp(data, "HOME");
	else if (double_array_len(data->argv) > 2)
		exit(EXIT_FAILURE);
	else
		dir = cd_parse(data);
	ft_printf("%s\n", dir);
	if (dir == NULL)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
	// chdir(dir);
	// getcwd(cwd, sizeof(cwd));
	// ft_printf("%s\n", cwd);
	// exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
