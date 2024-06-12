/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_type_handler.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/12 16:38:44 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


t_token	ms_type_handler_word(t_cmd *cmd, t_token token, int *pos)
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

t_token	ms_type_handler_rein(t_cmd *cmd, t_token token, int *pos)
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

t_token	ms_type_handler_reout(t_cmd *cmd, t_token token, int *pos)
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

t_token	ms_type_handler_append(t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_append: Token APPEND\n");
	t_token	append;
	int		outarr_size;

	*pos += token.length;
	*pos += ms_skipspace(&token.start);
	token.length = 0;
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

t_token	ms_type_handler_heredoc(t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) pos;
	printf("ms_type_handler_heredoc: Token heredoc");
	return (token);
}

t_token	ms_type_handler_eof(t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) pos;
	printf("ms_type_handler_eof: Token EOF");
	return (token);
}

t_token	ms_type_handler_error(t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) token;
	(void) pos;
	ms_error("Token error.");
	return (token);
}
