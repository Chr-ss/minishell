/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/24 15:15:56 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint ;

static void	read_heredoc(t_msdata *data, t_cmd *cmd, int i, int write_pipe)
{
	while (1)
	{
		if (data->line)
			free(data->line);
		data->line = NULL;
		data->line = readline(">");
		data->line = expand(data);
		if (!data->line)
			error("read_heredoc: expand malloc error.", data);
		if (ft_strcmp(data->line, cmd->heredoc[i]) == 0)
		{
			if (data->line)
				free(data->line);
			data->line = NULL;
			return ;
		}
		if (cmd->heredoc[i + 1] == NULL)
		{
			write(write_pipe, data->line, ft_strlen(data->line));
			write(write_pipe, "\n", 1);
		}
		if (data->line)
			free(data->line);
		data->line = NULL;
	}
}

static int	heredoc_cmds(t_msdata *data, t_cmd *cmd)
{
	int	i;
	int	fd[2];

	i = 0;
	if (!cmd->heredoc || !cmd->heredoc[0] || cmd->infd == -1)
		return (0);
	if (pipe(fd) == -1)
	{
		write(2, "heredoc_cmds: error opening pipe.\n", 34);
		return (-1);
	}
	cmd->infd = fd[0];
	while (cmd->heredoc[i])
	{
		read_heredoc(data, cmd, i, fd[1]);
		i++;
	}
	if (close(fd[1]) == -1)
	{
		write(2, "heredoc_cmds: error closing write_end.\n", 39);
		return (-1);
	}
	return (0);
}

int	heredoc(t_msdata *data)
{
	t_cmd	*cmd;

	cmd = data->cmd_head;
	while (cmd)
	{
		if (heredoc_cmds(data, cmd) == -1)
			return (-1);
		cmd = cmd->pipe;
	}
	return (0);
}
