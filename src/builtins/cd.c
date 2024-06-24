
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
#include "../../include/minishell.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

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

//psd == dot + slash + operand {./OPERAND}
char *cd_parse_dso(char *operand)
{
	char	*concat;
	int		dir_check;

	dir_check = 0;
	concat = ft_strjoin("./", operand);
	dir_check = check_dir(concat);
	ft_printf("concat %s\n", concat);
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
	ft_printf("dir check dirname %s\n", *dirname);
	if (!*dirname)
		return (-1);
	dir_check = check_dir(*dirname);
	ft_printf("check_dir %d\n", dir_check);
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
		ft_printf("before dirname %s\n", dirname);
		ret = cd_parse_pso_dir_check(&dirname, operand);
		ft_printf("after dirname %s\n", dirname);
		ft_printf("ret %d\n", ret);
		if (ret == 0 || ret == -1)
			break;
		index++;
	}
	//free array
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
		ft_printf("CDPATH: %s\n", cdpath);
		if (cdpath != NULL)
			cdpath = cd_parse_pso(cdpath, data->argv[1]);
	}
	if (cdpath == NULL)
		cdpath = cd_parse_dso(data->argv[1]);
	ft_printf("wat\n");
	return (cdpath);
}
//TEST CASES 
// pso: export CDPATH=.:~/projects/core_projects/minishell/src && mkdir -p ~/projects/core_projects/minishell/src/lol cd ~/projects && cd lol && unset CDPATH
// dso: mkdir -p ~/projects/core_projects/minishell/src/lol && cd ~/projects/core_projects/minishell/src && cd lol
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
	{
		dir = cd_parse(data);
		ft_printf("lol\n");
	}
	ft_printf("dir %s\n", dir);
	if (dir == NULL)
		exit(EXIT_FAILURE);
	chdir(dir);
	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	free(dir);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
