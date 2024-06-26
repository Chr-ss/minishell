/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:41:51 by spenning          #+#    #+#             */
/*   Updated: 2024/06/28 17:29:02 by spenning         ###   ########.fr       */
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

int	cd_chdir(t_msdata *data, char *dir)
{
	char	*pwd;
	char	*old_pwd;

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
// pso: make re && export CDPATH=.:~/projects/core_projects/minishell/src 
// && mkdir -p ~/projects/core_projects/minishell/src/lol cd ~/projects 
// && valgrind --leak-check=full cd lol && unset CDPATH
// dso: mkdir -p ~/projects/core_projects/minishell/src/lol 
// && cd ~/projects/core_projects/minishell/src && cd lol
// canonical form test Original Path: ///a/./b/../c//d/e/../ ; 
// ///a/b/../c//d/e/../; ///a/c//d/e/../; ///a/c//d/; /a/c/d
// cd $(echo $(perl -E 'say "/" x 5000')"home/spenning") works in cd, 
// but does not work here because of PATH_MAX

// add to debug
// char	cwd[PATH_MAX];
// ft_printf("before PWD %s\n", get_envp(data, "PWD"));
// ft_printf("before OLDPWD%s\n", get_envp(data, "OLDPWD"));
// ft_printf("after PWD %s\n", get_envp(data, "PWD"));
// ft_printf("after OLDPWD%s\n", get_envp(data, "OLDPWD"));
// getcwd(cwd, sizeof(cwd));
// ft_printf("%s\n", cwd);

int	cd(t_msdata *data)
{
	char	*dir;
	int		arglen;

	dir = NULL;
	if (ft_strlen (data->argv[1]) > PATH_MAX)
		error("path too long for cd");
	arglen = double_array_len(data->argv);
	if (arglen == 1)
		dir = get_envp(data, "HOME");
	else if (arglen > 2)
		error("too many arguments for cd");
	else
		dir = cd_parse(data);
	if (dir == NULL)
		error("allocation error");
	if (cd_chdir(data, dir))
		error("chdir error");
	return (EXIT_SUCCESS);
}
