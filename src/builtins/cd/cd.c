/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/18 14:41:51 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/02 15:54:57 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory
//-exists
//https://stackoverflow.com/questions/291828/what-is-the-best-way-to-return-an-
//error-from-a-function-when-im-already-returni
//https://stackoverflow.com/questions/9314586/c-faster-way-to-check-if-a-
//directory-exists
//https://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

int	cd_chdir_free(int ret, char *pwd, char *old_pwd)
{
	if (pwd)
		free(pwd);
	if (old_pwd)
		free(old_pwd);
	return (ret);
}

char	*cd_chdir_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*ret;

	if (!getcwd(cwd, PATH_MAX))
		return (NULL);
	ret = ft_strdup(cwd);
	if (ret == NULL)
		return (NULL);
	return (ret);
}

int	cd_chdir(t_msdata *data, char *dir)
{
	char	*pwd;
	char	*old_pwd;

	if (get_envp(data, "PWD", &old_pwd) == -1)
		error ("cd_chdir get_envp error", data);
	pwd = cd_chdir_pwd();
	if (!pwd || !old_pwd)
	{
		if (pwd)
			free(pwd);
		if (old_pwd)
			free(old_pwd);
		return (EXIT_FAILURE);
	}
	if (chdir(dir) == -1)
		return (cd_chdir_free(EXIT_FAILURE, pwd, old_pwd));
	if (change_envp("PWD", pwd, data->envp) == -1)
		return (cd_chdir_free(EXIT_FAILURE, pwd, old_pwd));
	if (change_envp("OLDPWD", old_pwd, data->envp) == -1)
	{
		if (change_envp("PWD", old_pwd, data->envp) == -1)
			return (cd_chdir_free(EXIT_FAILURE, pwd, old_pwd));
	}
	return (cd_chdir_free(EXIT_SUCCESS, pwd, old_pwd));
}

int	cd_error(int arglen, char **argv)
{
	if (ft_strlen(argv[0]) > PATH_MAX)
	{
		write(2, "path too long for cd\n", 21);
		return (1);
	}
	else if (arglen > 1)
	{
		write(2, " too many arguments\n", 20);
		return (1);
	}
	return (0);
}

int	cd(t_msdata *data, char **argv)
{
	char	*dir;
	int		arglen;

	dir = NULL;
	arglen = double_array_len(argv);
	if (arglen != 0 && cd_error(arglen, argv))
		return (1);
	if (arglen == 0)
	{
		if (get_envp(data, "HOME", &dir) == -1)
			error("cd get_envp error", data);
	}
	else
		dir = cd_parse(data, argv);
	if (cd_parse_oldpwd(data, argv, dir))
		return (1);
	if (cd_chdir(data, dir))
		error("chdir error", data);
	if (dir)
		free(dir);
	return (EXIT_SUCCESS);
}
