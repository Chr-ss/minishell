/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_parse_check.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/05 19:36:52 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/05 19:41:10 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

void cd_parse_pso_check(char **dirname)
{
	if (check_dir(*dirname))
	{
		free(*dirname);
		*dirname = NULL;
	}
}
