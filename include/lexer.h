/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 14:57:39 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LEXER_H
# define	LEXER_H

# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	char			*in;
	char			*out;
	struct	s_cmd	*next;
}	t_cmd;

typedef enum e_token_type
{
	WORD,
	PIPE,
	third
}	t_token_type;

#endif	// LEXER_H
