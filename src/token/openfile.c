/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   openfile.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:29:43 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/24 20:16:00 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	openfile(t_msdata *data, t_token token, int open_flag, int *fd)
{
	int		new_fd;
	char	*filename;

	if (*fd < 0)
		return ;
	filename = ft_strndup(token.start, token.length);
	if (!filename)
		error("openfile: malloc error.", data);
	new_fd = open(filename, open_flag, 0644);
	if (*fd > 0)
	{
		close(*fd);
		*fd = 0;
	}
	if (*fd == 0)
		*fd = new_fd;
	if (new_fd < 0)
		perror(filename);
	free(filename);
}
