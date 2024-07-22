/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_oldpwd.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 13:08:58 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/22 20:34:00 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*cd_parse_oldpwd(t_msdata *data)
{
	int		ret;
	char	*cdpath;

	ret = get_envp(data, "OLDPWD", &cdpath);
	if (ret == -1)
		error("cdparse getenvp error", data);
	ft_printf("%s\n", cdpath);
	return (cdpath);
}
