/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 17:01:17 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LEXER_H
# define	LEXER_H

# include <readline/readline.h>
# include <readline/history.h>

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

typedef struct s_token
{
	char			*start;
	t_token_type	type;
	int				length;
}	t_token;

typedef struct s_tokenizer
{
	char	*start;
	char	*curr;
}	t_tokenizer;


#endif	// LEXER_H
