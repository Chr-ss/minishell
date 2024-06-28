/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:43:01 by spenning          #+#    #+#             */
/*   Updated: 2024/06/28 17:19:09 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_char_array(char **arr)
{
	int	index;

	index = 0;
	while (arr[index] != NULL)
	{
		free(arr[index]);
		arr[index] = 0;
		index++;
	}
	free(arr);
	arr = NULL;
}

void	copy_over_str(int dst_index, int src_index, char**dst, char **src)
{
	dst[dst_index] = ft_strdup(src[src_index]);
	if (!dst[dst_index])
	{
		free_char_array(dst);
		ms_error("copy_over_str, malloc error");
	}
}
