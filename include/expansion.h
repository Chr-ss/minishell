/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/04 17:19:02 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

typedef struct s_expend
{
	char			*env;
	char			*line;
	int				line_pos;
	int				capacity;
}	t_expend;

#endif	// EXPANSION_H
