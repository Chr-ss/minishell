/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isalpha.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/02 16:54:02 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 14:43:05 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/libft.h"

int	ft_isalpha(int c)
{
	if ((97 <= c && c <= 122) || (65 <= c && c <= 90))
		return (1);
	return (0);
}