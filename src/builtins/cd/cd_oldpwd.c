/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_oldpwd.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 13:08:58 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 15:53:35 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	cd_parse_oldpwd(t_msdata *data, char **argv, char *cdpath)
{
	int		ret;

	if (argv && !ft_strncmp(argv[0], "-", ft_strlen(argv[0])) && \
	cdpath == NULL)
	{
		ret = get_envp(data, "OLDPWD", &cdpath);
		if (ret == -1)
			error("cdparse getenvp error", data);
		if (ret == 1)
		{
			write(STDERR_FILENO, "minishell: OLDPWD not set\n", 26);
			return (ret);
		}
		ft_printf("%s\n", cdpath);
		return (ret);
	}
	else if (cdpath == NULL)
		write(STDERR_FILENO, "minishell: No such file or directory\n", 37);
	return (0);
}
