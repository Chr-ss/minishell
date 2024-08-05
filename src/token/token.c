/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/05 21:40:05 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/03 11:08:27 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	quoted_length(char *line, char quote)
{
	int	q_len;
	int	i;

	q_len = 0;
	while (line[q_len] && line[q_len + 1] && line[q_len + 1] != quote)
	{
		line[q_len] = line[q_len + 1];
		q_len++;
	}
	i = q_len;
	while (line[i + 2])
	{
		line[i] = line[i + 2];
		i++;
	}
	line[i] = '\0';
	line[i + 1] = '\0';
	return (q_len);
}

static int	tokenizer_redirections(t_token *token)
{
	if (!*token->start)
		token->type = TOKEN_EOF;
	else if (!ft_strncmp(token->start, "|", 1))
		token->type = TOKEN_PIPE;
	else if (!ft_strncmp(token->start, "<<", 2))
	{
		token->type = TOKEN_HEREDOC;
		token->length = 2;
	}
	else if (!ft_strncmp(token->start, ">>", 2))
	{
		token->type = TOKEN_APPEND;
		token->length = 2;
	}
	else if (!ft_strncmp(token->start, "<", 1))
		token->type = TOKEN_REIN;
	else if (!ft_strncmp(token->start, ">", 1))
		token->type = TOKEN_REOUT;
	else
		return (0);
	return (1);
}

t_token	tokenizer(char *line)
{
	t_token	tkn;

	tkn.start = line;
	tkn.length = 1;
	tkn.type = TOKEN_ERROR;
	if (tokenizer_redirections(&tkn))
		return (tkn);
	else if (tkn.start && *tkn.start && ft_isprint(*tkn.start) \
			&& !ft_isbashtoken(*tkn.start) && \
			!ft_isspace(*tkn.start))
	{
		tkn.type = TOKEN_WORD;
		tkn.length = 0;
		while (tkn.start[tkn.length] && ft_isprint(tkn.start[tkn.length]) \
				&& !ft_isbashtoken(tkn.start[tkn.length]) && \
				!ft_isspace(tkn.start[tkn.length]))
		{
			if (tkn.start[tkn.length] == '\'' || tkn.start[tkn.length] == '"')
				tkn.length += quoted_length(&tkn.start[tkn.length], \
				tkn.start[tkn.length]);
			else
				tkn.length++;
		}
	}
	return (tkn);
}

int	line_to_token(t_msdata *data, char *line)
{
	t_token	token;

	token.start = NULL;
	token.length = 0;
	token.type = 0;
	while (token.type != TOKEN_EOF)
	{
		data->pos = skipspace(line, data->pos);
		token = tokenizer(&line[data->pos]);
		token = token_to_cmd(data, token, &data->pos);
		if (token.type == TOKEN_ERROR)
		{
			cmd_reset(data);
			return (-1);
		}
		data->pos += token.length;
	}
	debugger("\n\n\n");
	return (0);
}
