/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_utils_chriss.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/01 15:01:31 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/20 17:31:04 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ms_skipspace(char *str, int pos)
{
	while (str && str[pos] && ft_isspace(str[pos]))
		pos++;
	return (pos);
}
