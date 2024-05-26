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
	struct	s_cmd	*pipe;
}	t_cmd;

typedef enum e_token_type
{
	TOKEN_WORD = 1,
	TOKEN_PIPE,
	TOKEN_REIN,
	TOKEN_REOUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

#endif	// LEXER_H
