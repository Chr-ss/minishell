/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_readline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/01 19:06:28 by crasche       ########   odam.nl         */
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

void	ms_expansion_exp_init(t_expend *exp, char *line)
{
	exp->line = line;
	exp->exp_var = NULL;
	exp->exp_len = 0;
	exp->exp_str = NULL;
	exp->new_line = NULL;
	exp->nl_pos = 0;
	exp->capacity = DYNSTRING;
}

void	ms_expansion_var_nl(t_expend *exp)
{
	while (*exp->exp_str)
	{
		if (exp->nl_pos == exp->capacity)
		{
			exp->new_line = str_expand_new(exp->new_line, &exp->capacity);
			if(!exp->new_line)
				ms_error("ms_expesion, malloc error.");
		}
		exp->new_line[exp->nl_pos] = *exp->exp_str;
			exp->exp_str++;
			exp->nl_pos++;
	}
}
// char	*find_env(char *str, char **env, int length)
// {
// 	int	i;

// 	i = 0;
// 	while(env[i])
// 	{
// 		if(ft_strncmp())

// 		i++;
// 	}
// }

void	ms_expansion_var(t_expend *exp)
{
	char	*str;

	exp->exp_var = exp->line;
	while(ft_isalnum((int) *exp->line))
	{
		exp->line++;
		exp->exp_len++;
	}
	// find_endv(exp->exp_var, copied_env, exp->exp_len);
	str = ft_strndup(exp->exp_var, exp->exp_len); //you do not need
	if (!str)
		ms_error("ms_expansion_getlen, alloc error.");
	exp->exp_str = getenv(str); //you need to use find_env;
	free(str);
	exp->exp_var = NULL;
	exp->exp_len = 0;
	ms_expansion_var_nl(exp);
}

void	ms_expansion_copy(t_expend *exp)
{
	while (*exp->line)
	{
		if (exp->nl_pos == exp->capacity)
		{
			exp->new_line = str_expand_new(exp->new_line, &exp->capacity);
			if(!exp->new_line)
				ms_error("ms_expesion, malloc error.");
		}
		if (*exp->line != '$')
		{
			exp->new_line[exp->nl_pos] = *exp->line;
			exp->line++;
			exp->nl_pos++;
		}
		else if (*exp->line == '$' && ft_isdigit((int) exp->line[1]))
		{
			exp->line++;
			exp->line++;
		}
		else if (*exp->line == '$')
		{
			exp->line++;
			ms_expansion_var(exp);
		}
	}
}

char *ms_expesion(char **line)
{
	t_expend	exp;

	ms_expansion_exp_init(&exp, *line);
	if (str_expand(&exp.new_line, &exp.capacity) == -1)
		ms_error("ms_expesion, malloc error.");
	ms_expansion_copy(&exp);
	free(*line);
	*line = NULL;
	return (exp.new_line);
}


char	*ms_readline(t_msdata *data, char **argv)
{
	char *line;

	while (1)
	{
		(void) argv;
		line = readline("minishell:~$");
		line = ms_expesion(&line);
		if (!line)
			ms_error("readline malloc error.");
		printf(">>>%s\n\n", line);
		ms_error("FINISHED!!\n\n");
		temp_print_tokens(data, line);
		free(line);
		line = NULL;
	}
	return (line);
}
