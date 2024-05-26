/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/05 12:30:25 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 14:43:06 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned char		*d;
	unsigned const char	*s;

	d = (unsigned char *) dst;
	s = (unsigned const char *) src;
	if (size <= 0)
		return (ft_strlen(src));
	else if (ft_strlen(src) == 0)
	{
		*d = '\0';
		return (ft_strlen(src));
	}
	while (--size && *s != '\0')
		*d++ = *s++;
	*d = '\0';
	return (ft_strlen(src));
}
