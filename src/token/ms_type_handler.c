/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_type_handler.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/20 17:30:53 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


t_token	ms_type_handler_word(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_word: Token WORD\n");

	(void) data;
	(void) pos;
	if (!cmd->cmd)
	{
		cmd->cmd = ft_strndup(token.start, (size_t) token.length);
		if (!cmd->cmd)
			ms_error("ms_type_handler_word: malloc error.");
	}
	else
	{
		cmd->argv = ms_extend_strarr(cmd, cmd->argv, ms_strarr_size(cmd->argv));
		ms_token_to_strarr(data, cmd->argv, token);
	}
	return (token);
}

t_token	ms_type_handler_pipe(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_pipe: Token pipe");

	data->cmd_curr->pipe = ft_calloc(1, sizeof(t_cmd));
	data->cmd_curr = data->cmd_curr->pipe;

	(void) cmd;
	(void) pos;
	return (token);
}

t_token	ms_type_handler_rein(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_rein: Token REIN\n");
	t_token	infile;

	*pos += token.length;
	*pos = ms_skipspace(data->line, *pos);
	token.length = 0;
	infile = ms_tokenizer(&(data->line[*pos]));
	if (infile.type != TOKEN_WORD)
		ms_unexpected_token(data, infile);
	else
	{
		ms_openfile(cmd, infile, O_TRUNC, &cmd->infd);
		// if (cmd->in)
		// 	free(cmd->in);
		// cmd->in = ft_strndup(infile.start, infile.length);
		// if (!cmd->in)
		// 	ms_error("ms_type_handler_rein: malloc error.");
	}
	return (infile);
}

t_token	ms_type_handler_reout(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_reout: Token REOUT\n");
	t_token	outfile;
	// int		outarr_size;

	*pos += token.length;
	*pos = ms_skipspace(data->line, *pos);
	token.length = 0;
	outfile = ms_tokenizer(&(data->line[*pos]));
	if (outfile.type != TOKEN_WORD)
		ms_unexpected_token(data, outfile);
	else
	{
		ms_openfile(cmd, outfile, O_TRUNC, &cmd->outfd);
		// if (cmd->append && cmd->out)
		// 	ms_clear_append(cmd);
		// outarr_size = ms_strarr_size(cmd->out);
		// cmd->out = ms_extend_strarr(cmd, cmd->out, outarr_size);
		// ms_token_to_strarr(data, cmd->out, outfile);
	}
	return (outfile);
}

t_token	ms_type_handler_append(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_append: Token APPEND\n");
	t_token	append;
	// int		outarr_size;

	*pos += token.length;
	*pos = ms_skipspace(data->line, *pos);
	token.length = 0;
	append = ms_tokenizer(&(data->line[*pos]));
	if (append.type != TOKEN_WORD)
		ms_unexpected_token(data, append);
	else
	{
		ms_openfile(cmd, append, O_APPEND, &cmd->outfd);
		// if (cmd->append && cmd->out)
		// 	ms_clear_append(cmd);
		// outarr_size = ms_strarr_size(cmd->out);
		// cmd->out = ms_extend_strarr(cmd, cmd->out, outarr_size);
		// ms_token_to_strarr(data, cmd->out, append);
		// cmd->append = true;
	}
	return (append);
}

t_token	ms_type_handler_heredoc(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	printf("ms_type_handler_reout: Token REOUT\n");
	t_token	heredoc;

	*pos += token.length;
	*pos = ms_skipspace(data->line, *pos);
	token.length = 0;
	heredoc = ms_tokenizer(&(data->line[*pos]));
	if (heredoc.type != TOKEN_WORD)
		ms_unexpected_token(data, heredoc);
	else
	{
		cmd->heredoc = ms_extend_strarr(cmd, cmd->heredoc, ms_strarr_size(cmd->heredoc));
		ms_token_to_strarr(data, cmd->heredoc, heredoc);
	}
	return (heredoc);
}

t_token	ms_type_handler_eof(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) data;
	(void) pos;
	printf("ms_type_handler_eof: Token EOF");
	return (token);
}

t_token	ms_type_handler_error(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	(void) cmd;
	(void) data;
	(void) token;
	(void) pos;
	ms_error("Token error.");
	return (token);
}
