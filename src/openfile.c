/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   openfile.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/13 14:22:39 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	openfile(t_cmd *cmd, t_token token, int open_flag, int *fd)
{
	int		new_fd;
	char	*filename;

	filename = ft_strndup(token.start, token.length);
	new_fd = open(filename, O_RDWR, open_flag);
	if (new_fd == -1)
	{
		printf("Can not open file.\n");
	}
	if (*fd)
	{
		close(*fd);
		*fd = 0;
	}
	if (*fd == 0)
	{
		*fd = new_fd;
	}
	if (*fd < 0 && new_fd > 0)
	{
		close (new_fd);
	}
	free(filename);
	(void) cmd;
}