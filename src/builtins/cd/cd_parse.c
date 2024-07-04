/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_parse.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 19:15:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/04 15:36:43 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

//cd functions
//dso == dot + slash + operand {./OPERAND}
char	*cd_parse_dso(char *operand)
{
	char	*concat;
	int		dir_check;

	dir_check = 0;
	concat = ft_strjoin("./", operand);
	dir_check = check_dir(concat);
	if (dir_check == EXIT_FAILURE)
		return (NULL);
	else if (dir_check == EXIT_SUCCESS)
		return (concat);
	else
		return (NULL);
	return (NULL);
}

//pso == pathname + slash + operand {CDPATH/OPERAND}
char	*cd_parse_pso_dir_check_malloc(char *dirname, char *operand)
{
	char	*temp;

	dirname = ft_strdup(dirname);
	if (dirname == NULL)
		return (NULL);
	if (dirname[ft_strlen(dirname) - 1] != '/')
	{
		temp = ft_strjoin(dirname, "/");
		if (temp == NULL)
			return (NULL);
		free(dirname);
		dirname = ft_strjoin(temp, operand);
		free(temp);
	}
	return (dirname);
}

int	cd_parse_pso_dir_check(char **dirname, char *operand)
{
	int		dir_check;

	*dirname = cd_parse_pso_dir_check_malloc(*dirname, operand);
	if (!*dirname)
		return (-1);
	dir_check = check_dir(*dirname);
	if (dir_check == 0)
		return (0);
	else if (dir_check == 1)
		return (1);
	else
	{
		free(*dirname);
		*dirname = NULL;
		return (-1);
	}
}

char	*cd_parse_pso(char *cdpath, char *operand)
{
	char	**sp_cdpath;
	int		index;
	char	*dirname;
	int		ret;

	ret = 0;
	index = 0;
	dirname = NULL;
	sp_cdpath = ft_split(cdpath, ':');
	if (sp_cdpath == NULL)
		return (NULL);
	free(cdpath);
	while (sp_cdpath[index] != NULL)
	{
		dirname = sp_cdpath[index];
		ret = cd_parse_pso_dir_check(&dirname, operand);
		if (ret == 0 || ret == -1)
			break ;
		index++;
	}
	free_char_array(sp_cdpath);
	return (dirname);
}

char	*cd_parse(t_msdata *data)
{
	char	operand[2];
	char	*cdpath;
	int		ret;

	cdpath = NULL;
	ft_strlcpy(operand, data->argv[1], 2);
	if (!ft_strncmp(data->argv[1], "-", ft_strlen(data->argv[1])))
	{
		ret = get_envp(data, "OLDPWD", &cdpath);
		if (ret == -1)
			error("cdparse getenvp error");
		return (cdpath);
	}
	if (operand[0] == '/')
		return (data->argv[1]);
	else if (operand[0] == '.' || !ft_strncmp(operand, "..", 2))
		return (data->argv[1]);
	else
	{
		ret = get_envp(data, "CDPATH", &cdpath);
		if (cdpath != NULL && ret != -1)
			cdpath = cd_parse_pso(cdpath, data->argv[1]);
	}
	if (cdpath == NULL && ret == 1)
		cdpath = cd_parse_dso(data->argv[1]);
	return (cdpath);
}
