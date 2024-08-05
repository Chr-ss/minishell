/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:43:42 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 12:03:30 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	env(t_msdata *data, char **argv)
{
	int	index;
	int	arglen;

	arglen = double_array_len(argv);
	if (arglen > 0)
	{
		write(STDERR_FILENO, "env\n", 4);
		return (127);
	}
	index = 0;
	while (data->envp[index] != NULL)
	{
		if (ft_strchr(data->envp[index], '='))
			ft_printf("%s\n", data->envp[index]);
		index++;
	}
	return (0);
}
