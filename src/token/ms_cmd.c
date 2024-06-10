/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_cmd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/05 21:56:01 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/06 20:14:59 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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