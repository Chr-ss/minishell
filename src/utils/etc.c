/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   etc.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/24 18:44:04 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/09 10:27:50 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	double_array_len(char **vector)
{
	int	index;

	if (vector == NULL)
		return (0);
	index = 0;
	while (vector[index] != NULL)
		index++;
	return (index);
}
