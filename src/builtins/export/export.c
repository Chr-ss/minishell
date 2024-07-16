/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/16 18:23:52 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
// export
// export lol
// export lol lol lol
// export lol=wat
// export=
// export lol=wat wat wat
// export wat=lol=wat {wat='lol=wat'}
// export wat= {wat="", echo $wat = ""}
// export [existing variable]=new_value 
// {[existing variable]=new_value, echo $[existing variable] = new_value}

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

int export_print_env(t_msdata *data)
{
	int		index;
	int		inner_index;
	int		len;
	int		inner_len;
	char	*temp;

	index = 0;
	inner_index = 1;
	len = 0;
	inner_len = 0;
	while (data->envp[index] != NULL)
	{
		while (data->envp[inner_index] != NULL)
		{
			len = ft_strlen(data->envp[index]);
			inner_len = ft_strlen(data->envp[inner_index]);
			if (inner_len > len)
				len = inner_len;
			if (ft_strncmp(data->envp[inner_index], data->envp[index], len) > 0)
			{
				temp = data->envp[index];
				data->envp[index] = data->envp[inner_index];
				data->envp[inner_index] = temp;
			}
			inner_index++;
		}
		inner_index = 0;
		index++;
	}
	ft_printf("%s\n", data->envp[index]);
	index = 0;
	while (data->envp[index] != NULL)
		ft_printf("declare -x %s\n", data->envp[index++]);
	return (0);
}

/**
 * @brief To check if first character is a digit, export does not accept this
 * 
 * @param argv 
 * @return int 
 */
int export_check_identifier(char *argv)
{
	int index;

	index = 0;
	if (argv)
	{
		if (!(ft_isalpha(argv[0])))
		{
			dprintf(2, "minishell: export: not a valid identifier\n");
			return (1);
		}
		while (argv[index] != '=' && argv[index] != '\0')
		{
			if (!(ft_isalnum(argv[index])))
			{
				dprintf(2, "minishell: export: not a valid identifier\n");
				// perror("not a valid identifier test");
				return (1);
			}
			index++;
		}
	}
	return (0);
}

int	export(t_msdata *data, char **argv)
{
	char	*value;
	char	*key;
	int		check_envp;
	int		index;

	index = 0;
	if (!argv)
		return (export_print_env(data));
	while (argv && argv[index])
	{
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
		debugger("check_envp %d\n", check_envp);
		if (check_envp != -1)
			unset(data, NULL, key);
		add_envp(data, key, value);
		index++;
	}
	return (0);
}
