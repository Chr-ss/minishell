/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_parse.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 19:15:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/02 15:55:26 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

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
	{
		free(concat);
		return (NULL);
	}
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

char	*cd_parse(t_msdata *data, char **argv)
{
	char	op[2];
	char	*cdpath;
	int		ret;

	cdpath = NULL;
	ft_strlcpy(op, argv[0], 2);
	if (op[0] == '/' || op[0] == '.' || !ft_strncmp(op, "..", 2))
	{
		cdpath = ft_strdup(argv[0]);
		if (cdpath == NULL)
			error("cdpath malloc", data);
		return (cdpath);
	}
	else
	{
		ret = get_envp(data, "CDPATH", &cdpath);
		if (cdpath != NULL && ret != -1)
			cdpath = cd_parse_pso(cdpath, argv[0]);
	}
	if (cdpath == NULL && ret == 1)
		cdpath = cd_parse_dso(argv[0]);
	return (cdpath);
}
