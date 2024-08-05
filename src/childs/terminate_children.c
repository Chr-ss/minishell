/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   terminate_children.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/01 19:04:45 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/05 20:06:21 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	kill_all_childs(t_msdata *data)
{
	t_childs	*temp;

	temp = data->childs;
	while (temp != NULL)
	{
		if (temp->pid != 0)
			kill(temp->pid, SIGTERM);
		temp = temp->next;
	}
}
