/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:59:21 by spenning          #+#    #+#             */
/*   Updated: 2024/06/26 20:16:43 by spenning         ###   ########.fr       */
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

int		char_index(char *str, char chr)
{
	int index;

	index = 0;
	while (str[index] != '\0')
	{
		if (str[index] == chr)
			return (index);
		index++;
	}
	return (-1);
}

char	*export_get_key(char *input)
{
	int		index;
	char	*key;

	index = 0;
	while (input[index] != '\0' || input[index] != '=')
		index++;
	key = ft_calloc((index + 1), 1);
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
	while (input[start] != '=')
		start++;
	if (start == len)
	{
		value = ft_strdup("");
		if (value == NULL)
			return (NULL);
		return (value);
	}
	end = start;
	while(input[end] != '\0')
		end++;
	value = ft_calloc(((end - start) + 1), 1);
	if (value == NULL)
		return (NULL);
	ft_strlcpy(value, input + start, end - start);
	return (value);
}

void	export(t_msdata *data)
{
	char	*value;
	char	*key;

	if (double_array_len(data->argv) == 1)
		return (env(data));
	key = export_get_key(data->argv[2]);
	value = export_get_value(data->argv[2]);
	ft_printf("key %s\n", key);
	ft_printf("value %s\n", value);
	// add_envp(key, value);
}

