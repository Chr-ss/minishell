/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_readline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/27 18:17:55 by veno          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_isbashtoken(int c)
{
	if (c == '|' || c == '>' || c == '<' || c == 0)
		return (1);
	return (0);
}

void	skipspace(char **string)
{
	while (ft_isspace((int) **string))
		(*string)++;
}

t_token	ms_tokenizer(char *line)
{
	t_token	token;
	char	*temp;

	token.start = line;
	temp = token.start;
	token.length = 1;
	if (!*token.start)
	{
		printf("EOF, ");
		token.type = TOKEN_EOF;
	}
	else if (!ft_strncmp(token.start, "|", 1))
	{
		printf("PIPE, ");
		token.type = TOKEN_PIPE;
	}
	else if (!ft_strncmp(token.start, "<<", 2))
	{
		printf("HEREDOC, ");
		token.type = TOKEN_HEREDOC;
		token.length = 2;
	}
	else if (!ft_strncmp(token.start, "<", 1))
	{
		printf("REIN, ");
		token.type = TOKEN_REIN;
	}
	else if (!ft_strncmp(token.start, ">>", 2))
	{
		printf("APPEND, ");
		token.type = TOKEN_APPEND;
		token.length = 2;
	}
	else if (!ft_strncmp(token.start, ">", 1))
	{
		printf("REOUT, ");
		token.type = TOKEN_REOUT;
	}
	else if (*temp && ft_isprint((int) *temp) && !ft_isbashtoken((int) *temp))
	{
		printf("WORD, ");
		token.type = TOKEN_WORD;
		token.length = 0;
		while (*temp && ft_isprint((int) *temp) && !ft_isbashtoken((int) *temp))
		{
			temp++;
			token.length++;
		}
	}
	else
		token.type = TOKEN_ERROR;
	return (token);
}

char *ms_malloc_word_token(t_token
{

}

int	ms_type_handler_word(t_cmd *cmd, t_token token)
{
	if (!cmd->cmd)
		ms_token
}

int	ms_type_handler_pipe(t_cmd *cmd, t_token token)
{

}

int	ms_type_handler_rein(t_cmd *cmd, t_token token)
{

}

int	ms_type_handler_reout(t_cmd *cmd, t_token token)
{

}

int	ms_type_handler_append(t_cmd *cmd, t_token token)
{

}

int	ms_type_handler_heredoc(t_cmd *cmd, t_token token)
{
	printf("ms_type_handler_heredoc: Token heredoc");
	return (0);
}

int	ms_type_handler_eof(t_cmd *cmd, t_token token)
{
	printf("ms_type_handler_eof: Token EOF");
	return (0);
}

int	ms_type_handler_error(t_cmd *cmd, t_token token)
{
	(void) data;
	(void) token;
	ms_error("Token error.");
	return (0);
}

int	ms_token_to_cmd(t_msdata *data, t_token token)
{
	t_cmd	*cmd;
	int		(*type_handler[8])(t_cmd *cmd, t_token token);

	type_handler[0] = ms_type_handler_word;
	type_handler[1] = ms_type_handler_pipe;
	type_handler[2] = ms_type_handler_rein;
	type_handler[3] = ms_type_handler_reout;
	type_handler[4] = ms_type_handler_append;
	type_handler[5] = ms_type_handler_heredoc;
	type_handler[6] = ms_type_handler_eof;
	type_handler[7] = ms_type_handler_error;
	cmd = data->cmd;




}


void	temp_print_tokens(t_msdata *data, char *line)
{
	t_token	token;
	int	i = 0;

	token.start = line;
	while (token.type != TOKEN_EOF)
	{
		skipspace(&token.start);
		token = ms_tokenizer(token.start);
		printf("%d, ", token.type);
		i++;
		if (token.type == TOKEN_EOF)
			return ;
		while (token.length)
		{
			token.length--;
			token.start++;
		}
		ms_token_to_cmd(data, token);
	}
	printf("\n");
}

char	*ms_readline(t_msdata *data)
{
	char *line;

	while (1)
	{
		write(1, "\n", 1);
		line = readline("minishell:~$");
		if (!line)
			ms_error("readline malloc error.");
		printf("INPUT:%s\n\n", line);
		temp_print_tokens(data, line);
		free(line);
		line = NULL;
	}
	return (line);
}
