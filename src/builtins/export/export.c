/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 12:03:34 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

char	*export_check_value(char *value)
{
	int		index;
	char	*temp;

	index = 0;
	while (value[index] != '\0')
	{
		if (value[index] == '=')
		{
			temp = ft_strjoin("'", value);
			free(value);
			if (temp == NULL)
				return (NULL);
			value = ft_strjoin(temp, "'");
			free(temp);
			if (value == NULL)
				return (NULL);
			return (value);
		}
		index++;
	}
	return (value);
}

/**
 * @brief To check if first character is a digit, export does not accept this
 *
 * @param argv
 * @return int
 */
int	export_check_identifier(char *argv)
{
	int	index;

	index = 0;
	if (argv)
	{
		if (!(ft_isalpha(argv[0])) && argv[0] != '_')
		{
			write(STDERR_FILENO, "minishell: export: not a valid identifier\n", 42);
			return (1);
		}
		while (argv[index] != '=' && argv[index] != '\0')
		{
			if (!(ft_isalnum(argv[index])) && argv[index] != '_')
			{
				write(STDERR_FILENO, "minishell: export: not a valid identifier\n", 42);
				return (1);
			}
			index++;
		}
	}
	return (0);
}

int	export_parse(t_msdata *data, char **argv, int index)
{
	char	*key;
	char	*value;
	int		check_envp;

	if (export_check_identifier(argv[index]))
		return (1);
	key = get_envp_key(argv[index]);
	if (key == NULL)
		error("malloc error in envp_get_key in export", data);
	value = get_envp_value(argv[index]);
	if (value == NULL)
		error("malloc error in envp_get_value in export", data);
	value = export_check_value(value);
	if (value == NULL)
		error("malloc error in export_check_value", data);
	check_envp = get_envp_index(key, data->envp);
	if (check_envp != -1)
		unset(data, NULL, key);
	add_envp(data, key, value);
	if (value)
		free(value);
	if (key)
		free(key);
	return (0);
}

int	export(t_msdata *data, char **argv)
{
	int		index;

	index = 0;
	if (!argv)
		return (export_print_env(data));
	while (argv && argv[index])
	{
		if (export_parse(data, argv, index))
			return (1);
		index++;
	}
	return (0);
}
