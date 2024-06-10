/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_readline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/10 18:23:45 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int g_sigint ;

int	ft_isbashtoken(int c)
{
	if (c == '|' || c == '>' || c == '<' || c == 0)
		return (1);
	return (0);
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

int	ms_strarr_size(char **strarr)
{
	int	i;

	i = 0;
	while (strarr && strarr[i])
		i++;
	return (i);
}

char	**ms_extend_strarr(t_cmd *cmd, char **strarr, int strarr_size)
{
	(void) cmd;
	int		i;
	char	**new_strarr;

	i = 0;
	strarr_size++;
	new_strarr = calloc(strarr_size + 1, sizeof(char*));
	if (!new_strarr)
		ms_error("ms_extend_strarr: malloc error.");
	while (strarr && strarr[i])
	{
		new_strarr[i] = strarr[i];
		strarr[i] = NULL;
		i++;
	}
	free(strarr);
	return (new_strarr);
}
void	ms_token_to_strarr(t_cmd *cmd, char **strarr, t_token token)
{
	(void) cmd;
	int	i;

	i = 0;
	while (strarr[i])
		i++;
	strarr[i] = ft_strndup(token.start, (size_t) token.length);
	if (!strarr[i])
		ms_error("ms_extend_strarr: malloc error.");
}

// void	skipspace(char **string)
// {
// 	while (ft_isspace((int) **string))
// 		(*string)++;
// }

void	ms_l(t_msdata *data, char *line)
{
	t_token	token;
	int		pos;

	pos = 0;
	while (token.type != TOKEN_EOF)
	{
		token.start = &line[pos];
		token.length = 0;
		pos = ms_skipspace(line, pos);
		token = ms_tokenizer(&line[pos]);
		// printf("%d, ", token.type);
		// i++;
		token = ms_token_to_cmd(data, token);
		pos += token.length;
		if (token.type == TOKEN_EOF)
			break ;
	}
	printf("\n");
	printf_cmd(data->cmd);
}

void	ms_readline(t_msdata *data)
{
	while (1)
	{
		data->line = readline("minishell:~$");
		if (!data->line)
			ms_error("seb made if for this...");
		data->line = ms_expand(data);
		if (!data->line)
			ms_error("readline malloc error.");
		printf(">>>%s\n\n", data->line);
		temp_print_tokens(data, data->line);
		ms_parsing(data);
		free(data->line);
		data->line = NULL;
	}
}
