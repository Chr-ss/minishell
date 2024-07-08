/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/05 18:46:17 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//unset $PWD && ./minishell pwd should still work

int	pwd(t_msdata *data)
{
	char	cwd[PATH_MAX];

	if (double_array_len(data->argv) > 1)
		return (1);
	if (!getcwd(cwd, PATH_MAX))
		return (1);
	ft_printf("%s\n", cwd);
	return (0);
}
