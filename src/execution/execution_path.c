/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_path.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/04 11:49:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/23 18:02:46 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	if (ret == 0)
	{
		str = ft_strdup(cmd);
		if (str == NULL)
			return (-1);
		*path_cmd = str;
		return (1);
	}
	return (0);
}

int	execute_path(char	*cmd, t_msdata *data, char **path_cmd)
{
	int		ret;
	char	*path;
	char	**path_spl;

	ret = execute_path_local(cmd, path_cmd);
	if (ret == -1 || ret == 1)
		return (ret);
	ret = get_envp(data, "PATH", &path);
	if (ret == -1 || ret == 1)
		return (ret);
	path_spl = ft_split(path, ':');
	free(path);
	if (!path_spl)
		return (-1);
	ret = execute_path_check(path_spl, cmd, path_cmd);
	debugger("ret: %d\n", ret);
	free_char_array(path_spl);
	if (*path_cmd == NULL && ret == -1)
		return (-1);
	else if (ret == 1)
		return (1);
	return (0);
}
