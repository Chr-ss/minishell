/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_argv.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/04 11:49:12 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/10 15:06:24 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_path_check(char **path_spl, char *cmd, char **path_cmd)
{
	int		index;
	char	*temp;
	char	*temp_cmd;
	int		check;

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
		check = access(temp_cmd, X_OK);
		if (check == 0)
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
	else if (ret == -1)
		return (-1);
	return(0);
}



int	execute_path(char	*cmd, t_msdata *data, char **path_cmd)
{
	int		ret;
	char	*path;
	char	**path_spl;

	ret = execute_path_local(cmd, path_cmd);
	if (ret == -1)
		return (-1);
	else if (ret == 1)
		return (0);
	ret = get_envp(data, "PATH", &path);
	if (ret == -1)
		return (-1);
	else if (ret == 1)
		return (1);
	path_spl = ft_split(path, ':');
	if (!path_spl)
	{
		if (path)
			free(path);
		return (-1);
	}
	ret = execute_path_check(path_spl, cmd, path_cmd);
	free_char_array(path_spl);
	if (*path_cmd == NULL && ret == -1)
		return (-1);
	else if (ret == 1)
		return (1);
	return (0);
}

int	add_command_to_argv(t_cmd	**cmd_s, char **path_cmd)
{
	int		index;
	int		arglen;
	char	**new_argv;
	char	*path;

	index = 0;
	arglen = double_array_len((*cmd_s)->argv);
	new_argv = ft_calloc(sizeof(char *) * (arglen + 2), 1);
	if (new_argv == NULL)
		return (-1);
	new_argv[arglen + 1] = NULL;
	path = ft_strdup(path_cmd[0]);
	if (path == NULL)
	{
		free_char_array(new_argv);
		return (-1);
	}
	new_argv[0] = path;
	while ((*cmd_s)->argv && (*cmd_s)->argv[index] != NULL)
	{
		copy_over_str(index + 1, index, new_argv, (*cmd_s)->argv);
		index++;
	}
	free_char_array((*cmd_s)->argv);
	(*cmd_s)->argv = new_argv;
	return (0);
}
