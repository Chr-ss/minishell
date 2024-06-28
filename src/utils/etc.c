/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:44:04 by spenning          #+#    #+#             */
/*   Updated: 2024/06/24 18:53:14 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	double_array_len(char **vector)
{
	int	index;

	index = 0;
	while (vector[index] != NULL)
		index++;
	return (index);
}
