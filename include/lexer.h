/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 21:12:18 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/31 17:08:46 by crasche       ########   odam.nl         */
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
	char			*start;
	int				length;
	char			*var;
	int				var_len;
	int				line_chars;
}	t_expend;

typedef struct s_linedata
{
	char			*line;
	int				line_pos;
	char			*exp_line;
	int				exp_pos;
	int				exp_len;
}	t_linedata;

typedef struct s_tokenizer
{
	char	*start;
	char	*curr;
}	t_tokenizer;

#endif	// LEXER_H
