/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:40:05 by crasche           #+#    #+#             */
/*   Updated: 2024/07/03 19:59:52 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	return_quoted_length(char *line, char quote)
{
	int	quoted_len;
	int	i;

	quoted_len = 0;
	while (line[quoted_len] && line[quoted_len + 1] && line[quoted_len + 1] != quote)
	{
		line[quoted_len] = line[quoted_len + 1];
		quoted_len++;
	}
	i = quoted_len;
	while (line[i + 2])
	{
		line[i] = line[i + 2];
		i++;
	}
	line[i] = '\0';
	line[i + 1] = '\0';
	return (quoted_len);
}

t_token	tokenizer(char *line)
{
	t_token	token;
	char	*temp;

	token.start = line;
	temp = token.start;
	token.length = 1;
	if (!*token.start)
	{
		token.type = TOKEN_EOF;
		write(1, "EOF, ", 5);
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else if (!ft_strncmp(token.start, "|", 1))
	{
		token.type = TOKEN_PIPE;
		write(1, "PIPE, ", 6);
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else if (!ft_strncmp(token.start, "<<", 2))
	{
		token.type = TOKEN_HEREDOC;
		token.length = 2;
		write(1, "HEREDOC, ", 9);
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else if (!ft_strncmp(token.start, "<", 1))
	{
		token.type = TOKEN_REIN;
		write(1, "REIN, ", 6);
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else if (!ft_strncmp(token.start, ">>", 2))
	{
		token.type = TOKEN_APPEND;
		token.length = 2;
		write(1, "APPEND, ", 8);
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else if (!ft_strncmp(token.start, ">", 1))
	{
		write(1, "REOUT, ", 7);
		token.type = TOKEN_REOUT;
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else if (token.start && *token.start && ft_isprint((int) *token.start) && !ft_isbashtoken((int) *token.start) && !ft_isspace((int) *token.start))
	{
		token.type = TOKEN_WORD;
		token.length = 0;
		while (temp[token.length] && ft_isprint((int) temp[token.length]) && !ft_isbashtoken((int) temp[token.length]) && !ft_isspace((int) temp[token.length]))
		{
			if(temp[token.length] == '\'' || temp[token.length] == '"')
				token.length += return_quoted_length(&temp[token.length], temp[token.length]);
			token.length++;
		}
		write(1, "WORD, ", 6);
		write(1, &token.start[0], token.length);
		write(1, "$\n", 2);
	}
	else
		token.type = TOKEN_ERROR;
	return (token);
}

void	temp_print_tokens(t_msdata *data, char *line)
{
	t_token	token;

	while (token.type != TOKEN_EOF)
	{
		data->pos = skipspace(line, data->pos);
		token = tokenizer(&line[data->pos]);
		token = token_to_cmd(data, token, &data->pos);
		data->pos += token.length;
		if (token.type == TOKEN_EOF)
			break ;
	}
	printf("\n");
	printf_cmd(data->cmd_head);
	execute(data);
}