/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_readline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/31 17:24:05 by crasche       ########   odam.nl         */
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

t_token	ms_type_handler_word(t_cmd *cmd, t_token token)
{
	printf("ms_type_handler_word: Token WORD\n");
	int	argv_size;

	if (!cmd->cmd)
	{
		cmd->cmd = ft_strndup(token.start, (size_t) token.length);
		if (!cmd->cmd)
			ms_error("ms_type_handler_word: malloc error.");
	}
	else
	{
		argv_size = ms_strarr_size(cmd->argv);
		cmd->argv = ms_extend_strarr(cmd, cmd->argv, argv_size);
		ms_token_to_strarr(cmd, cmd->argv, token);
	}
	return (token);
}

t_token	ms_type_handler_pipe(t_cmd *cmd, t_token token)
{
	(void) cmd;
	printf("ms_type_handler_pipe: Token pipe");
	return (token);
}

void	ms_unexpected_token(t_cmd *cmd, t_token token)
{
	(void) cmd;
	write(2, "-minishell: syntax error near unexpected token '", 48);
	write(2, token.start, token.length);
	write(2, "'", 1);
	ms_error("");
}

t_token	ms_type_handler_rein(t_cmd *cmd, t_token token)
{
	printf("ms_type_handler_rein: Token REIN\n");
	t_token	infile;

	while (token.length)
	{
		token.length--;
		token.start++;
	}
	skipspace(&token.start);
	infile = ms_tokenizer(token.start);
	if (infile.type != TOKEN_WORD)
		ms_unexpected_token(cmd, infile);
	else
	{
		if (cmd->in)
			free(cmd->in);
		cmd->in = ft_strndup(infile.start, infile.length);
		if (!cmd->in)
			ms_error("ms_type_handler_rein: malloc error.");
	}
	return (infile);
}

void	ms_clear_append(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->append = false;
	while (cmd->out && cmd->out[i] && cmd->out[i + 1])
		i++;
	free(cmd->out[i]);
	cmd->out[i] = NULL;
}

t_token	ms_type_handler_reout(t_cmd *cmd, t_token token)
{
	printf("ms_type_handler_reout: Token REOUT\n");
	t_token	outfile;
	int		outarr_size;

	while (token.length)
	{
		token.length--;
		token.start++;
	}
	skipspace(&token.start);
	outfile = ms_tokenizer(token.start);
	if (outfile.type != TOKEN_WORD)
		ms_unexpected_token(cmd, outfile);
	else
	{
		if (cmd->append && cmd->out)
			ms_clear_append(cmd);
		outarr_size = ms_strarr_size(cmd->out);
		cmd->out = ms_extend_strarr(cmd, cmd->out, outarr_size);
		ms_token_to_strarr(cmd, cmd->out, outfile);
	}
	return (outfile);
}

t_token	ms_type_handler_append(t_cmd *cmd, t_token token)
{
	printf("ms_type_handler_append: Token APPEND\n");
	t_token	append;
	int		outarr_size;

	while (token.length)
	{
		token.length--;
		token.start++;
	}
	skipspace(&token.start);
	append = ms_tokenizer(token.start);
	if (append.type != TOKEN_WORD)
		ms_unexpected_token(cmd, append);
	else
	{
		if (cmd->append && cmd->out)
			ms_clear_append(cmd);
		outarr_size = ms_strarr_size(cmd->out);
		cmd->out = ms_extend_strarr(cmd, cmd->out, outarr_size);
		ms_token_to_strarr(cmd, cmd->out, append);
		cmd->append = true;
	}
	return (append);
}

t_token	ms_type_handler_heredoc(t_cmd *cmd, t_token token)
{
	(void) cmd;
	printf("ms_type_handler_heredoc: Token heredoc");
	return (token);
}

t_token	ms_type_handler_eof(t_cmd *cmd, t_token token)
{
	(void) cmd;
	printf("ms_type_handler_eof: Token EOF");
	return (token);
}

t_token	ms_type_handler_error(t_cmd *cmd, t_token token)
{
	(void) cmd;
	(void) token;
	ms_error("Token error.");
	return (token);
}

// void ms_init_type_handler(t_token type_handler[])
// {
// 	(*type_handler)[0] = ms_type_handler_word;
// 	(*type_handler)[1] = ms_type_handler_pipe;
// 	(*type_handler)[2] = ms_type_handler_rein;
// 	(*type_handler)[3] = ms_type_handler_reout;
// 	(*type_handler)[4] = ms_type_handler_append;
// 	(*type_handler)[5] = ms_type_handler_heredoc;
// 	(*type_handler)[6] = ms_type_handler_eof;
// 	(*type_handler)[7] = ms_type_handler_error;
// }
void	ms_init_type_handler(t_token (*type_handler[8])(t_cmd *cmd, t_token token))
{
	type_handler[0] = ms_type_handler_word;
	type_handler[1] = ms_type_handler_pipe;
	type_handler[2] = ms_type_handler_rein;
	type_handler[3] = ms_type_handler_reout;
	type_handler[4] = ms_type_handler_append;
	type_handler[5] = ms_type_handler_heredoc;
	type_handler[6] = ms_type_handler_eof;
	type_handler[7] = ms_type_handler_error;
}

t_token	ms_token_to_cmd(t_msdata *data, t_token token)
{
	t_cmd	*cmd;

	t_token (*type_handler[8])(t_cmd * cmd, t_token token);
	cmd = data->cmd;
	ms_init_type_handler(type_handler);
	token = type_handler[token.type](cmd, token);
	return (token);
}

	// printf("test %s", type_handler[0]);
	// cmd = data->cmd;
	// type_handler[0] = ms_type_handler_word;
	// type_handler[1] = ms_type_handler_pipe;
	// type_handler[2] = ms_type_handler_rein;
	// type_handler[3] = ms_type_handler_reout;
	// type_handler[4] = ms_type_handler_append;
	// type_handler[5] = ms_type_handler_heredoc;
	// type_handler[6] = ms_type_handler_eof;
	// type_handler[7] = ms_type_handler_error;

void	printf_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	printf("\n\t ## CMD ## \n");
	while (cmd)
	{
		if (cmd->cmd)
			printf("CMD:%s\n", cmd->cmd);
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			printf("ARGV[%d]:%s\n", i, cmd->argv[i]);
			i++;
		}
		if (cmd->in)
			printf("INFILE:%s\n", cmd->in);
		i = 0;
		while (cmd->out && cmd->out[i])
		{
			printf("OUT[%d]:%s\n", i, cmd->out[i]);
			i++;
		}
		if (cmd->append)
			printf("APPEND:%d\n", cmd->append);
		cmd = cmd->pipe;
	}
	printf("\n\n");
}

void	temp_print_tokens(t_msdata *data, char *line)
{
	t_token	token;
	// int		i = 0;

	token.start = line;
	token.length = 0;
	while (token.type != TOKEN_EOF)
	{
		while (token.length)
		{
			token.length--;
			token.start++;
		}
		skipspace(&token.start);
		token = ms_tokenizer(token.start);
		// printf("%d, ", token.type);
		// i++;
		token = ms_token_to_cmd(data, token);
		if (token.type == TOKEN_EOF)
			break ;
	}
	printf("\n");
	printf_cmd(data->cmd);
}

void	ms_expansion_getlen(t_expend *exp_var)
{
	char	*str;

	str = ft_strndup(exp_var->start, exp_var->length);
	if (!str)
		ms_error("ms_expansion_getlen, alloc error.");
	exp_var->var = getenv(str);
	free(str);
	exp_var->length = ft_strlen(exp_var->var);
}

void	ms_expansion_data_init(t_linedata *data, char *line)
{
	data->line = line;
	data->line_pos = 0;
	data->exp_line = NULL;
	data->exp_pos = 0;
	data->exp_len = ft_strlen(line);
}

void	ms_expansion_var_init(t_expend *exp_var)
{
	exp_var->start = NULL;
	exp_var->length = 0;
	exp_var->var = NULL;
	exp_var->line_chars = 0;
}

void	ms_expansion_loopline(t_linedata *data);

void	ms_expansion_var(t_linedata *data, t_expend *exp_var)
{

	exp_var->start = &data->line[data->line_pos];
	exp_var->length = 0;
	while (data->line[data->line_pos] && data->line[data->line_pos] != '$' && ft_isalnum(data->line[data->line_pos]))
	{
		data->line_pos++;
		exp_var->length++;
	}
	ms_expansion_getlen(exp_var);
	exp_var->var_len = ft_strlen(exp_var->var);
	data->exp_len += exp_var->var_len - exp_var->length;
	ms_expansion_loopline(data);
}

void	ms_expansion_copy(t_linedata *data, t_expend exp_var)
{
	if (!data->exp_line)
		data->exp_line = ft_calloc(data->exp_len + 1, sizeof(char));
	if (!data->exp_line)
			ms_error("ms_expansion_copy malloc error.");
	while (exp_var.line_chars)
	{
		data->line_pos--;
		data->exp_line[data->exp_len] = data->line[data->line_pos];
		data->exp_len--;
		exp_var.line_chars--;
	}
	while (exp_var.var_len)
	{
		data->exp_line[data->exp_len] = exp_var.var[exp_var.var_len];
		data->exp_len--;
		exp_var.var_len--;
	}
	while (exp_var.length)
	{
		data->line_pos--;
		exp_var.length--;
	}
	printf("%s", data->exp_line);
}

void	ms_expansion_loopline(t_linedata *data)
{
	t_expend	exp_var;

	ms_expansion_var_init(&exp_var);
	while(data->line[data->line_pos])
	{
		if (data->line[data->line_pos] == '$')
		{
			data->line_pos++;
			if (ft_isalpha((int) data->line[data->line_pos])) //data->line[data->line_pos] != '$' &&
			{
				ms_expansion_var(data, &exp_var);
				break ;
			}
			else if (ft_isdigit((int) data->line[data->line_pos]))
			{
				exp_var.length = 2;
				break ;
			}
			else if (data->line[data->line_pos] == '?')
				break ;
			else
				exp_var.line_chars++;
		}
		else
		{
			data->line_pos++;
			exp_var.line_chars++;
		}
	}
	ms_expansion_copy(data, exp_var);
}

char *ms_expesion(char **argv, char **line)
{
	t_linedata	data;

	(void) argv;
	ms_expansion_data_init(&data, *line);
	ms_expansion_loopline(&data);
	free(*line);
	*line = NULL;
	return (data.exp_line);
}


char	*ms_readline(t_msdata *data, char **argv)
{
	char *line;

	while (1)
	{
		// write(1, "\n", 1);
		line = readline("minishell:~$");
		line = ms_expesion(argv, &line);
		// (void) argv;
		// printf("\n\ntest::: %s\n\n\n", getenv("PATH"));
		if (!line)
			ms_error("readline malloc error.");
		printf("INPUT:%s\n\n", line);
		temp_print_tokens(data, line);
		free(line);
		line = NULL;
	}
	return (line);
}
