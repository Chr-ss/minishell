/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_cmd.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/05 21:56:01 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/12 16:36:54 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_token_to_strarr(t_msdata *data, char **strarr, t_token token)
{
	(void) data;
	int	i;

	i = 0;
	while (strarr[i])
		i++;
	strarr[i] = ft_strndup(token.start, (size_t) token.length);
	if (!strarr[i])
		ms_error("ms_token_to_strarr: malloc error.");
}

void	ms_unexpected_token(t_msdata *data, t_token token)
{
	(void) data;
	write(2, "-minishell: syntax error near unexpected token '", 48);
	write(2, token.start, token.length);
	write(2, "'", 1);
	ms_error("");
}

// void	ms_clear_append(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	cmd->append = false;
// 	while (cmd->out && cmd->out[i] && cmd->out[i + 1])
// 		i++;
// 	free(cmd->out[i]);
// 	cmd->out[i] = NULL;
// }

void	ms_init_type_handler(t_token (*type_handler[8])(t_msdata *data, t_cmd *cmd, t_token token, int *pos))
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

t_token	ms_token_to_cmd(t_msdata *data, t_token token, int *pos)
{
	t_token (*type_handler[8])(t_msdata * data, t_cmd * cmd, t_token token, int *pos);
	ms_init_type_handler(type_handler);
	token = type_handler[token.type](data, data->cmd_curr, token, pos);
	return (token);
}
