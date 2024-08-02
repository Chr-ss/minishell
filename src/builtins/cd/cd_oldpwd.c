/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_oldpwd.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 13:08:58 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/02 15:57:49 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	cd_parse_oldpwd(t_msdata *data, char **argv, char *cdpath)
{
	int		ret;

	if (!ft_strncmp(argv[0], "-", ft_strlen(argv[0])) && cdpath == NULL)
	{
		ret = get_envp(data, "OLDPWD", &cdpath);
		if (ret == -1)
			error("cdparse getenvp error", data);
		if (ret == 1)
		{
			write(2, "minishell: OLDPWD not set\n", 26);
			return (ret);
		}
		ft_printf("%s\n", cdpath);
		return (ret);
	}
	else
		write(2, "minishell: No such file or directory\n", 37);
	return (1);
}
