/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_parsing.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:02:41 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 18:16:32 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_init_cmdlist(t_cmd *cmd)
{
	cmd = ft_calloc(sizeof(t_cmd), 1);
}

void	ms_parsing(t_msdata *data)
{
	ms_init_cmdlist(data->cmd);
}
