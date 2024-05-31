/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/31 19:10:10 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REIN,
	TOKEN_REOUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

typedef struct s_token
{
	char			*start;
	int				length;
	t_token_type	type;
}	t_token;

typedef struct s_expend
{
	char			*line;
	char			*exp_var;
	int				exp_len;
	char			*exp_str;
	int				exp_slen;
	char			*new_line;
	int				capacity;
}	t_expend;


typedef struct s_tokenizer
{
	char	*start;
	char	*curr;
}	t_tokenizer;

#endif	// LEXER_H
