/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:52:00 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 13:46:33 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error(char *msg, t_msdata *data)
{
	cmd_free(data);
	if (data->line)
	{
		free(data->line);
		data->line = NULL;
	}
	if (data->envp)
	{
		free_char_array(data->envp);
		data->envp = NULL;
	}
	perror(msg);
	exit(1);
}
