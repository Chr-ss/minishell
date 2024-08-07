/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/05 21:56:01 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/03 11:05:00 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	token_to_strarr(t_msdata *data, char **strarr, t_token token)
{
	int	i;

	i = 0;
	while (strarr[i])
		i++;
	strarr[i] = ft_strndup(token.start, (size_t) token.length);
	if (!strarr[i])
		error("token_to_strarr: malloc error.", data);
}

void	init_type_handler(t_token (*type_handler[8])(t_msdata *data, \
	t_cmd *cmd, t_token token, int *pos))
{
	type_handler[0] = type_handler_word;
	type_handler[1] = type_handler_pipe;
	type_handler[2] = type_handler_rein;
	type_handler[3] = type_handler_reout;
	type_handler[4] = type_handler_append;
	type_handler[5] = type_handler_heredoc;
	type_handler[6] = type_handler_eof;
	type_handler[7] = type_handler_error;
}

t_token	token_to_cmd(t_msdata *data, t_token token, int *pos)
{
	t_token (*type_handler[8])(t_msdata * data, \
		t_cmd * cmd, t_token token, int *pos);
	init_type_handler(type_handler);
	token = type_handler[token.type](data, data->cmd_curr, token, pos);
	return (token);
}
