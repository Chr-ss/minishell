/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:59:21 by spenning          #+#    #+#             */
/*   Updated: 2024/06/27 15:12:39 by spenning         ###   ########.fr       */
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

char	*export_get_key(char *input)
{
	int		index;
	char	*key;

	index = 0;
	while (input[index] != '\0')
	{
		if (input[index] == '=')
			break ;
		index++;
	}
	index++;
	key = ft_calloc(index, 1);
	if (key == NULL)
		return (NULL);
	ft_strlcpy(key, input, index);
	return (key);
}

char	*export_get_value(char *input)
{
	int		start;
	int		end;
	int		len;
	char	*value;

	start = 0;
	end = 0;
	len = ft_strlen(input);
	while (input[start] != '=' && input[start] != '\0')
		start++;
	if (start == len)
	{
		value = ft_strdup("");
		if (value == NULL)
			return (NULL);
		return (value);
	}
	end = ++start;
	while (input[end] != '\0')
		end++;
	value = ft_calloc(((end - start) + 1), 1);
	if (value == NULL)
		return (NULL);
	ft_strlcpy(value, input + start, (end - start) + 1);
	return (value);
}

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

	if (double_array_len(data->argv) == 1)
		return (env(data));
	key = export_get_key(data->argv[1]);
	if (key == NULL)
		ms_error("malloc error in export_get_key");
	value = export_get_value(data->argv[1]);
	if (value == NULL)
		ms_error("malloc error in export_get_value");
	value = export_check_value(value);
	if (value == NULL)
		ms_error("malloc error in export_check_value");
	ft_printf("key %s\n", key);
	ft_printf("value %s\n", value);
	add_envp(data, key, value);
}
