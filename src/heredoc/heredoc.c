/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/08 15:21:58 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sig;

static void	read_heredoc_clear_dataline(t_msdata *data)
{
	if (data->line)
		free(data->line);
	data->line = NULL;
}

static int	heredoc_line(t_msdata *data, t_cmd *cmd, int i, int write_pipe)
{
	if (ft_strcmp(data->line, cmd->heredoc[i]) == 0)
	{
		read_heredoc_clear_dataline(data);
		return (1);
	}
	if (cmd->heredoc[i + 1] == NULL)
	{
		write(write_pipe, data->line, ft_strlen(data->line));
		write(write_pipe, "\n", 1);
	}
	read_heredoc_clear_dataline(data);
	return (0);
}

static int	read_heredoc(t_msdata *data, t_cmd *cmd, int i, int *write_pipe)
{
	while (1)
	{
		read_heredoc_clear_dataline(data);
		init_signal(data, hd);
		dup2(STDIN_FILENO, FDMAX - 3);
		data->line = readline(">");
		debugger(BLU "Heredoc line : %s\n" RESET, data->line);
		if (!data->line)
		{
			dup2(FDMAX - 3, STDIN_FILENO);
			close(FDMAX - 3);
			cmd_close_fd(write_pipe);
			cmd_close_fd(&cmd->infd);
			cmd->infd = -1;
			init_signal(data, afterhd);
			debugger(BLU "here \n" RESET);
			return (1);
		}
		data->line = expand(data);
		if (!data->line)
			error("read_heredoc: expand malloc error.", data);
		if (heredoc_line(data, cmd, i, *write_pipe))
			return (0);
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
		write(STDERR_FILENO, "heredoc_cmds: error opening pipe.\n", 34);
		return (-1);
	}
	cmd->infd = fd[0];
	while (cmd->heredoc[i])
	{
		if (read_heredoc(data, cmd, i, &fd[1]))
			break ;
		i++;
	}
	if (fd[1] && close(fd[1]) == -1)
	{
		write(STDERR_FILENO, "heredoc_cmds: error closing write_end.\n", 39);
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
