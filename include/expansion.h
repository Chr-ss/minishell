/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/09 17:32:18 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

typedef struct s_expand
{
	char			*env;
	char			*line;
	int				line_pos;
	int				capacity;
}	t_expand;

#endif	// EXPANSION_H
