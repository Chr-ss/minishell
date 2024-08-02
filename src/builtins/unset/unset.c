/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 15:20:04 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/02 15:36:05 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	**unset_new_envp(t_msdata *data, int skip_index)
{
	int		index;
	int		old_index;
	int		envp_len;
	char	**new_envp;

	index = 0;
	old_index = 0;
	envp_len = double_array_len(data->envp);
	new_envp = ft_calloc(envp_len, sizeof(char *));
	if (new_envp == NULL)
		return (NULL);
	while (data->envp[old_index] != NULL)
	{
		if (old_index == envp_len)
			break ;
		if (old_index == skip_index)
			old_index++;
		if (old_index == envp_len)
			break ;
		if (copy_over_str(index, old_index, new_envp, data->envp))
			error("copy_over_str, malloc error", data);
		index++;
		old_index++;
	}
	return (new_envp);
}

int	unset(t_msdata *data, char	**argv, char	*arg)
{
	int		env_index;
	char	**new_envp;
	int		index;

	index = 0;
	if (!arg && !argv)
		return (0);
	while (argv && argv[index])
	{
		if (arg != NULL)
			env_index = get_envp_index(arg, data->envp);
		else
			env_index = get_envp_index(argv[index], data->envp);
		if (env_index == -1)
			return (0);
		new_envp = unset_new_envp(data, env_index);
		if (new_envp == NULL)
			return (-1);
		swap_envp(data, new_envp);
		index++;
	}
	return (0);
}
