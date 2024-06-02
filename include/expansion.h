/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/02 17:35:18 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

typedef struct s_expend
{
	char			*env;
	// int				env_len;
	char			*line;
	int				line_pos;
	int				capacity;
}	t_expend;

#endif	// EXPANSION_H
