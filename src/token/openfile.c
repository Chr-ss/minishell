/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   openfile.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/16 18:56:32 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	openfile(t_msdata *data, t_token token, int open_flag, int *fd)
{
	int		new_fd;
	char	*filename;

	filename = ft_strndup(token.start, token.length);
	if (!filename)
		error("openfile: malloc error.", data);
	new_fd = open(filename, O_CREAT | open_flag, 0644);
	if (new_fd == -1)
	{
		// write(STDERR_FILENO, filename, ft_strlen(filename));
		write(2, (const char*)filename, ft_strlen(filename));
		write(2, ": Permission denied\n", 20);
	}
	if (*fd)
	{
		close(*fd);
		*fd = 0;
	}
	if (*fd == 0)
		*fd = new_fd;
	if (*fd < 0 && new_fd > 0)
		close (new_fd);
	free(filename);
}
