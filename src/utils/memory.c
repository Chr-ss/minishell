/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:43:01 by spenning          #+#    #+#             */
/*   Updated: 2024/06/27 15:56:59 by spenning         ###   ########.fr       */
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

void	copy_over_str(int index, char**dst, char **src)
{
	
	dst[index] = ft_strdup(src[index]);
	if (!dst[index])
	{
		free_char_array(dst);
		ms_error("copy_over_str, malloc error");
	}
}