/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:59:21 by spenning          #+#    #+#             */
/*   Updated: 2024/06/28 17:29:31 by spenning         ###   ########.fr       */
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

void	export(t_msdata *data)
{
	char	*value;
	char	*key;
	int		check_envp;

	if (double_array_len(data->argv) == 1)
		return (env(data));
	key = get_envp_key(data->argv[1]);
	if (key == NULL)
		error("malloc error in envp_get_key in export");
	value = get_envp_value(data->argv[1]);
	if (value == NULL)
		error("malloc error in envp_get_value in export");
	value = export_check_value(value);
	if (value == NULL)
		error("malloc error in export_check_value");
	check_envp = get_envp_index(key, data->envp);
	if (check_envp != -1)
		unset(data, key);
	add_envp(data, key, value);
}
