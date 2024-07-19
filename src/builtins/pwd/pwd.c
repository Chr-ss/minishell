/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/19 19:45:48 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//unset $PWD && ./minishell pwd should still work
int	pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		return (1);
	ft_printf("%s\n", cwd);
	return (0);
}
