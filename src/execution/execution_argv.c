/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_argv.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 15:08:54 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/22 15:32:55 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_command_to_argv_copy(t_cmd	**cmd_s, char **new_argv)
{
	int	index;

	index = 0;
	while ((*cmd_s)->argv && (*cmd_s)->argv[index] != NULL)
	{
		if (copy_over_str(index + 1, index, new_argv, (*cmd_s)->argv))
		{
			free_char_array(new_argv);
			return (1);
		}
		index++;
	}
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
	if (add_command_to_argv_copy(cmd_s, new_argv))
		return (-1);
	free_char_array((*cmd_s)->argv);
	(*cmd_s)->argv = new_argv;
	return (0);
}
