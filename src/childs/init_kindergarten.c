/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_kindergarten.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/01 15:51:26 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/01 19:10:42 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_kindergarten(t_msdata *data)
{
	data->childs = ft_calloc(sizeof(t_childs), 1);
	if (data->childs == NULL)
		error("init_kindergarten: malloc error", data);
	data->childs->pid = 0;
	data->childs->next = NULL;
}
