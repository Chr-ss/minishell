/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/18 14:41:51 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/20 18:13:32 by spenning      ########   odam.nl         */
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
	free(pwd);
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
	char	*oldpwd;

	oldpwd = ft_strdup(data->pwd);
	if (oldpwd == NULL)
		error ("cd: oldpwd malloc error", data);
	pwd = NULL;
	if (chdir(dir) == -1)
	{
		perror("cd");
		return (cd_chdir_free(EXIT_FAILURE, pwd, oldpwd));
	}
	pwd = cd_chdir_pwd();
	if (pwd == NULL)
		error ("cd: pwd malloc error", data);
	if (change_envp("PWD", pwd, data))
		add_envp(data, "PWD", pwd);
	if (change_envp("OLDPWD", oldpwd, data))
		add_envp(data, "OLDPWD", oldpwd);
	free(data->pwd);
	data->pwd = ft_strdup(pwd);
	if (data->pwd == NULL)
		error("pwd to data->pwd malloc error", data);
	return (cd_chdir_free(EXIT_SUCCESS, pwd, oldpwd));
}

int	cd_error(int arglen, char **argv)
{
	if (ft_strlen(argv[0]) > PATH_MAX)
	{
		write(STDERR_FILENO, "path too long for cd\n", 21);
		return (1);
	}
	else if (arglen > 1)
	{
		write(STDERR_FILENO, " too many arguments\n", 20);
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
	if (cd_parse_oldpwd(data, argv, &dir))
		return (1);
	if (dir && cd_chdir(data, dir))
		return (EXIT_FAILURE);
	if (dir)
		free(dir);
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
