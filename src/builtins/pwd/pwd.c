/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:59:21 by spenning          #+#    #+#             */
/*   Updated: 2024/06/25 15:49:30 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//unset $PWD && ./minishell pwd should still work

void	pwd(t_msdata *data)
{
	char	cwd[PATH_MAX];

	if (double_array_len(data->argv) > 1)
		return ;
	if(!getcwd(cwd, PATH_MAX))
		return ;
	ft_printf("%s\n", cwd);
}
