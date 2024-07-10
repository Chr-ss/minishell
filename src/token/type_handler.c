/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   type_handler.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/09 17:54:31 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	type_handler_word(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	debugger("type_handler_word: Token WORD\n");
	(void) data;
	(void) pos;
	if (!cmd->cmd)
	{
		cmd->cmd = ft_strndup(token.start, (size_t) token.length);
		if (!cmd->cmd)
			error("type_handler_word: malloc error.");
	}
	else
	{
		cmd->argv = extend_strarr(cmd, cmd->argv, strarr_size(cmd->argv));
		token_to_strarr(data, cmd->argv, token);
	}
	return (token);
}

t_token	type_handler_pipe(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	debugger("type_handler_pipe: Token pipe");
	data->cmd_curr->pipe = ft_calloc(1, sizeof(t_cmd));
	data->cmd_curr = data->cmd_curr->pipe;
	(void) cmd;
	(void) pos;
	return (token);
}

t_token	type_handler_rein(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	infile;

	debugger("type_handler_rein: Token REIN\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	infile = tokenizer(&(data->line[*pos]));
	if (infile.type != TOKEN_WORD)
		unexpected_token(data, infile);
	else
	{
		openfile(cmd, infile, O_TRUNC, &cmd->infd);
		if (cmd->heredoc)
			free_char_array(cmd->heredoc);
		cmd->heredoc = NULL;
	}
	return (infile);
}

t_token	type_handler_reout(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	outfile;

	debugger("type_handler_reout: Token REOUT\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	outfile = tokenizer(&(data->line[*pos]));
	if (outfile.type != TOKEN_WORD)
		unexpected_token(data, outfile);
	else
	{
		openfile(cmd, outfile, O_TRUNC, &cmd->outfd);
	}
	return (outfile);
}

t_token	type_handler_append(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	append;

	debugger("type_handler_append: Token APPEND\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	append = tokenizer(&(data->line[*pos]));
	if (append.type != TOKEN_WORD)
		unexpected_token(data, append);
	else
	{
		openfile(cmd, append, O_APPEND, &cmd->outfd);
	}
	return (append);
}

t_token	type_handler_heredoc(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	heredoc;

	debugger("type_handler_reout: Token REOUT\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	heredoc = tokenizer(&(data->line[*pos]));
	if (heredoc.type != TOKEN_WORD)
		unexpected_token(data, heredoc);
	else
	{
		cmd->heredoc = extend_strarr(cmd, cmd->heredoc, strarr_size(cmd->heredoc));
		token_to_strarr(data, cmd->heredoc, heredoc);
		if (cmd->infd > 0)
		{
			close(cmd->infd);
			cmd->infd = 0;
		}
	}
	return (heredoc);
}

t_token	type_handler_eof(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) data;
	(void) pos;
	debugger("type_handler_eof: Token EOF");
	return (token);
}

t_token	type_handler_error(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) data;
	(void) token;
	(void) pos;
	error("Token error.");
	return (token);
}
