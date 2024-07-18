/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   type_handler.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/16 18:56:52 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	type_handler_word(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	// debugger("type_handler_word: Token WORD\n");
	if (!cmd->cmd)
	{
		cmd->cmd = ft_strndup(token.start, (size_t) token.length);
		if (!cmd->cmd)
			error("type_handler_word: malloc error.", data);
	}
	else
	{
		cmd->argv = extend_strarr(cmd->argv, double_array_len(cmd->argv));
		if (!cmd->argv)
			error("type_handler_word; malloc error", data);
		token_to_strarr(data, cmd->argv, token);
	}
	(void) pos;
	return (token);
}

t_token	type_handler_pipe(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	// debugger("type_handler_pipe: Token pipe");
	if (!data->cmd_curr->cmd)
		token = unexpected_token(data, token);
	data->cmd_curr->pipe = ft_calloc(1, sizeof(t_cmd));
	if (!data->cmd_curr->pipe)
		error("type_handler_pipe; malloc error", data);
	data->cmd_curr = data->cmd_curr->pipe;
	(void) cmd;
	(void) pos;
	return (token);
}

t_token	type_handler_eof(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	debugger("type_handler_eof: Token EOF");
	(void) cmd;
	(void) data;
	(void) pos;
	return (token);
}

t_token	type_handler_error(t_msdata *data, t_cmd *cmd, t_token token, int *pos)
{
	write (2, "Token error.\n", 12);
	(void) cmd;
	(void) data;
	(void) token;
	(void) pos;
	return (token);
}
