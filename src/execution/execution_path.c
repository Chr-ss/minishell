/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_path.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/04 11:49:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/02 16:56:27 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_path_file(char *cmd)
{
	struct stat	path_stat;
	int			ret;

	stat(cmd, &path_stat);
	ret = S_ISREG(path_stat.st_mode);
	debugger ("execute_path_file ret %d\n", ret);
	if (ret == 0)
	{
		write(2, "minishell: ", 11);
		ft_putstr_fd(cmd, 2);
		write(2, ": Is a directory\n", 17);
		return (1);
	}
	return (0);
}

int	execute_path_error(void)
{
	write(2, "minishell: command not found\n", 29);
	return (1);
}

int	execute_path_check(char **path_spl, char *cmd, char **path_cmd)
{
	int		index;
	char	*temp;
	char	*temp_cmd;

	index = 0;
	while (path_spl[index] != NULL)
	{
		temp = ft_strjoin(path_spl[index], "/");
		if (!temp)
			return (-1);
		temp_cmd = ft_strjoin(temp, cmd);
		free(temp);
		if (!temp_cmd)
			return (-1);
		if (!access(temp_cmd, X_OK))
		{
			*path_cmd = temp_cmd;
			return (0);
		}
		else
			free(temp_cmd);
		index++;
	}
	return (1);
}

int	execute_path_local(char	*cmd, char **path_cmd)
{
	int		ret;
	char	*str;

	ret = access(cmd, X_OK);
	debugger("ret value of execute path local %d\n", ret);
	if (ret == 0)
	{
		if (execute_path_file(cmd))
			return (0);
		str = ft_strdup(cmd);
		if (str == NULL)
			return (-1);
		*path_cmd = str;
		return (0);
	}
	if (cmd[0] == '/' || cmd[0] == '.' || !ft_strncmp(cmd, "..", 2))
	{
		write(2, "minishell: ", 11);
		ft_putstr_fd(cmd, 2);
		write(2, ": No such file or directory\n", 28);
		return (0);
	}
	return (1);
}

int	execute_path(char	*cmd, t_msdata *data, char **path_cmd)
{
	int		ret;
	char	*path;
	char	**path_spl;

	ret = execute_path_local(cmd, path_cmd);
	if (ret == -1 || ret == 0)
		return (ret);
	ret = get_envp(data, "PATH", &path);
	if (ret == -1 || ret == 1)
		return (ret);
	path_spl = ft_split(path, ':');
	free(path);
	if (!path_spl)
		return (-1);
	ret = execute_path_check(path_spl, cmd, path_cmd);
	free_char_array(path_spl);
	if (*path_cmd == NULL && ret == -1)
		return (-1);
	else if (ret == 1)
		return (execute_path_error());
	return (0);
}
