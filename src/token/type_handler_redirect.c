/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   type_handler_redirect.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/10 14:56:49 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 16:56:02 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	unexpected_token(t_msdata *data, t_token token)
{
	write(2, "-minishell: syntax error near unexpected token '", 48);
	write(2, token.start, token.length);
	write(2, "'", 1);
	token.start = NULL;
	token.length = 0;
	token.type = TOKEN_ERROR;
	return (token);
}

t_token	type_handler_rein(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	infile;

	// debugger("type_handler_rein: Token REIN\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	infile = tokenizer(&(data->line[*pos]));
	if (infile.type != TOKEN_WORD)
		infile = unexpected_token(data, infile);
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

	// debugger("type_handler_reout: Token REOUT\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	outfile = tokenizer(&(data->line[*pos]));
	if (outfile.type != TOKEN_WORD)
		outfile = unexpected_token(data, outfile);
	else
	{
		openfile(cmd, outfile, O_TRUNC, &cmd->outfd);
	}
	return (outfile);
}

t_token	type_handler_append(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	append;

	// debugger("type_handler_append: Token APPEND\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	append = tokenizer(&(data->line[*pos]));
	if (append.type != TOKEN_WORD)
		append = unexpected_token(data, append);
	else
	{
		openfile(cmd, append, O_APPEND, &cmd->outfd);
	}
	return (append);
}

t_token	type_handler_heredoc(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	t_token	heredoc;

	// debugger("type_handler_reout: Token REOUT\n");
	*pos += token.length;
	*pos = skipspace(data->line, *pos);
	token.length = 0;
	heredoc = tokenizer(&(data->line[*pos]));
	if (heredoc.type != TOKEN_WORD)
		heredoc = unexpected_token(data, heredoc);
	else
	{
		cmd->heredoc = extend_strarr(cmd->heredoc, double_array_len(cmd->heredoc));
		if (!cmd->heredoc)
			error("type_handler_heredoc; malloc error", data);
		token_to_strarr(data, cmd->heredoc, heredoc);
		if (cmd->infd > 0)
		{
			close(cmd->infd);
			cmd->infd = 0;
		}
	}
	return (heredoc);
}
