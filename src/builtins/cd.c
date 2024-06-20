
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
		ft_printf(" %s not set\n", envp);
	else
		path = get_envp_value(data->envp[index]);
	if (path != NULL)
	{
		path = ft_strdup(path);
		if (path == NULL)
			return (NULL);
	}
	return (path);
}

//check dir function
int	check_dir(char *dirname)
{
	DIR*	dir;

	dir = opendir(dirname);
	if (dir) 
	{
		closedir(dir);
		return (EXIT_SUCCESS);
	}
	else if (ENOENT == errno) 
		return (EXIT_FAILURE);
	else
		return (-1);
}

//psd == dot + slash + operand {./OPERAND}
char *cd_parse_dso(char *operand)
{
	char	*concat;
	int		dir_check;

	dir_check = 0;
	concat = ft_strjoin("./", operand);
	dir_check = check_dir(concat);
	if (dir_check == EXIT_FAILURE)
		return (NULL);
	else if (dir_check == EXIT_SUCCESS)
		return (concat);
	else
		return (NULL);
	return (NULL);
}

//psd == pathname + slash + operand {CDPATH/OPERAND}
char *cd_parse_pso(char *cdpath, char *operand)
{
	char	**sp_cdpath;
	int		index;
	char	*dirname;
	int		dir_check;

	dir_check = 0;
	index = 0;
	if(cdpath == NULL)
		return (NULL);
	sp_cdpath = ft_split(cdpath, ':');
	if (sp_cdpath == NULL)
		return (NULL);
	while (sp_cdpath[index] != NULL)
	{
		dirname = sp_cdpath[index];
		if (sp_cdpath[index][ft_strlen(sp_cdpath[index]) - 1] != '/')
		{
			dirname = ft_strjoin(sp_cdpath[index], "/");
			dirname = ft_strjoin(dirname, operand);
		}
		if (!dirname)
			return (NULL);
		dir_check = check_dir(dirname);
		if (dir_check == 0) 
			index ++;
		else if (dir_check == 1)
			return (dirname);
		else
			return (NULL);
	}
	return(NULL);
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
		cdpath = cd_parse_pso(get_envp(data, "CDPATH"), data->argv[1]);
	if (cdpath == NULL)
		cdpath = cd_parse_dso(data->argv[1]);
	return (cdpath);
}
//TODO test CDPATH=.:~/projects/core_projects/minishell/src with a directory at that path location while calling cd in another directory

// change dir to be malloced always
int cd (t_msdata *data)
{
	char	*dir;
	char	cwd[PATH_MAX];
	int		arglen;

	dir = NULL;
	arglen = double_array_len(data->argv); 
	ft_printf("%d\n", arglen);
	if (arglen == 1)
		dir = get_envp(data, "HOME");
	else if (arglen > 2)
		exit(EXIT_FAILURE);
	else
		dir = cd_parse(data);
	ft_printf("here\n");
	ft_printf("%s\n", dir);
	if (dir == NULL)
		exit(EXIT_FAILURE);
	chdir(dir);
	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	free(dir);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
