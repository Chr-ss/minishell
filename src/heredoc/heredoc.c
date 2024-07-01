/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 16:17:52 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/20 17:30:53 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int g_sigint ;

void	heredoc_test(int read_pipe)
{
	char *temp;

	temp = get_next_line(read_pipe);
	while (temp)
	{
		ft_printf("%s", temp);
		free(temp);
		temp = NULL;
		temp = get_next_line(read_pipe);
	}
}

void	read_heredoc(t_msdata *data, char *delim, int write_pipe)
{
	while (1)
	{
		data->line = readline(">");
		if (!data->line)
		{
			write(1, "EMPTY. EXIT HEREDOC.", 19);
			return ;
		}
		data->line = expand(data);
		if (!data->line)
			error("input_handling malloc error.");
		printf(">>>%s|%s\n\n", data->line, delim);
		if (ft_strncmp(data->line, delim, ft_strlen(delim)) == 0)
		{
			if (data->line)
				free(data->line);
			return ;
		}
		write(write_pipe, data->line, ft_strlen(data->line));
		write(write_pipe, "\n", 1);
		if (data->line)
			free(data->line);
		data->line = NULL;
	}
}

void	heredoc_cmds(t_msdata *data, t_cmd *cmd)
{
	int	i;
	int	fd[2];

	i = 0;
	if (!cmd->heredoc || !cmd->heredoc[0] || cmd->infd == -1)
		return ;
	if (pipe(fd) == -1)
		error("Error opening pipe.");
	cmd->infd = fd[0];
	while (cmd->heredoc[i])
	{
		read_heredoc(data, cmd->heredoc[i], fd[1]);
		i++;
	}
	if (close(fd[1]) == -1)
		error("Error closing write_end heredoc.");
	heredoc_test(cmd->infd);
}

void	heredoc(t_msdata *data)
{
	t_cmd *cmd;

	cmd = data->cmd_head;
	while (cmd)
	{
		heredoc_cmds(data, cmd);
		cmd = cmd->pipe;
	}
}