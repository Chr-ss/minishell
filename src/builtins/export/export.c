/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:59:21 by spenning          #+#    #+#             */
/*   Updated: 2024/06/26 15:50:14 by spenning         ###   ########.fr       */
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
	while (str[index] != NULL)
	{
		if (str[index] == chr)
			return (index);
		index++;
	}
	return (-1);
}

char	*export_get_key(char *input)
{
	int	index;
	
}

void	export(t_msdata *data)
{
	char	*value;
	char	*key;
	char	*value;

	if (double_array_len(data->argv) == 1)
		return (env(data));
	key = export_get_key(data->argv[2]);
	value = export_get_value(data->argv[2]);
	add_envp(key, value);
}

