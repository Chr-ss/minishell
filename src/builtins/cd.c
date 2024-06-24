
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
//https://stackoverflow.com/questions/9314586/c-faster-way-to-check-if-a-directory-exists
//https://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
#include "../../include/minishell.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

// utils
void	free_char_array(char **arr)
{
	int	index;

	index = 0;
	while (arr[index] != NULL)
	{
		free(arr[index]);
		arr[index] = 0;
		index++;
	}
	free(arr);
	arr = NULL;
}

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

int change_envp(char *key, char *env, char **envp)
{
	int index;
	char *temp;
	char *concat;

	index = get_envp_index(key, envp);
	if (index == -1)
		return (EXIT_FAILURE);
	temp = ft_strjoin(key, "=");
	if (temp == NULL)
		return (EXIT_FAILURE);
	concat = ft_strjoin(temp, env);
	free(temp);
	if (concat == NULL)
		return (EXIT_FAILURE);
	temp = envp[index];
	envp[index] = concat;
	free(temp);
	return (EXIT_SUCCESS);
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

int	check_dir(char *dirname)
{
	struct stat s;

	int err = stat(dirname, &s);
	if(err == -1)
	{
		if(ENOENT == errno)
			return (EXIT_FAILURE);
		else
			return (-1);
	} 
	else 
	{
		if(S_ISDIR(s.st_mode)) 
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

//cd functions
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

//pso == pathname + slash + operand {CDPATH/OPERAND}
char	*cd_parse_pso_dir_check_malloc(char *dirname, char *operand)
{
	char	*temp;

	dirname = ft_strdup(dirname);
	if (dirname == NULL)
		return (NULL);
	if (dirname[ft_strlen(dirname) - 1] != '/')
	{
		temp = ft_strjoin(dirname, "/");
		if	(temp == NULL)
			return (NULL);
		free(dirname);
		dirname = ft_strjoin(temp, operand);
		free(temp);
	}
	return (dirname);
}


int cd_parse_pso_dir_check(char **dirname, char *operand)
{
	int		dir_check;

	*dirname = cd_parse_pso_dir_check_malloc(*dirname, operand);
	if (!*dirname)
		return (-1);
	dir_check = check_dir(*dirname);
	if (dir_check == 0)
		return (0);
	else if (dir_check == 1)
		return (1);
	else
	{
		free(*dirname);
		*dirname = NULL;
		return (-1);
	}
}


char *cd_parse_pso(char *cdpath, char *operand)
{
	char	**sp_cdpath;
	int		index;
	char	*dirname;
	int		ret;

	ret = 0;
	index = 0;
	sp_cdpath = ft_split(cdpath, ':');
	if (sp_cdpath == NULL)
		return (NULL);
	free(cdpath);
	while (sp_cdpath[index] != NULL)
	{
		dirname = sp_cdpath[index];
		ret = cd_parse_pso_dir_check(&dirname, operand);
		if (ret == 0 || ret == -1)
			break;
		index++;
	}
	free_char_array(sp_cdpath);
	return (dirname);
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
		cdpath = get_envp(data, "CDPATH");
		if (cdpath != NULL)
			cdpath = cd_parse_pso(cdpath, data->argv[1]);
	}
	if (cdpath == NULL)
		cdpath = cd_parse_dso(data->argv[1]);
	return (cdpath);
}

int cd_chdir (t_msdata *data, char *dir)
{
	char *pwd;
	char *old_pwd;

	old_pwd = get_envp(data, "PWD");
	pwd = ft_strdup(dir);
	if (!pwd || !old_pwd)
	{
		if (pwd)
			free(pwd);
		if (old_pwd)
			free(old_pwd);
		return (EXIT_FAILURE);
	}
	if (chdir(dir) == -1)
		return (EXIT_FAILURE);
	if (change_envp("PWD", pwd, data->envp) == -1)
		return (EXIT_FAILURE);
	if (change_envp("OLDPWD", old_pwd, data->envp) == -1)
	{
		if (change_envp("PWD", old_pwd, data->envp) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

//TEST CASES 
// pso: make re && export CDPATH=.:~/projects/core_projects/minishell/src && mkdir -p ~/projects/core_projects/minishell/src/lol cd ~/projects && cd lol && unset CDPATH
// dso: mkdir -p ~/projects/core_projects/minishell/src/lol && cd ~/projects/core_projects/minishell/src && cd lol
// canonical form test Original Path: ///a/./b/../c//d/e/../ ; ///a/b/../c//d/e/../; ///a/c//d/e/../; ///a/c//d/; /a/c/d
// cd $(echo $(perl -E 'say "/" x 5000')"home/spenning") works in cd, but does not work here because of PATH_MAX

int cd (t_msdata *data)
{
	char	*dir;
	char	cwd[PATH_MAX];
	int		arglen;

	dir = NULL;
	if (ft_strlen (data->argv[1]) > PATH_MAX)
		ms_error("path too long for cd");
	arglen = double_array_len(data->argv); 
	ft_printf("%d\n", arglen);
	if (arglen == 1)
		dir = get_envp(data, "HOME");
	else if (arglen > 2)
		ms_error("too many arguments for cd");
	else
		dir = cd_parse(data);
	if (dir == NULL)
		ms_error("allocation error");
	if (cd_chdir(data, dir))
		ms_error("chdir error");
	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	free(dir);
	return (EXIT_SUCCESS);
}
