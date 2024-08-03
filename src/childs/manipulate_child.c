/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manipulate_child.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/01 19:05:43 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 13:02:36 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_childs	*get_last_child(t_msdata *data)
{
	t_childs	*last;

	last = NULL;
	if (data->childs)
		last = data->childs;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

t_childs	*get_second_last_child(t_msdata *data)
{
	t_childs	*last;

	last = NULL;
	if (data->childs->next)
		last = data->childs;
	while (last->next->next != NULL)
		last = last->next;
	return (last);
}

void	add_child(int pid, t_msdata *data)
{
	t_childs	*new_child;
	t_childs	*end;

	new_child = ft_calloc(sizeof(t_childs), 1);
	if (new_child == NULL)
		error("add child: malloc error", data);
	end = get_last_child(data);
	if (end != NULL)
	{
		debugger(RED "Parent: Adding %d to child linked list\n" RESET, pid);
		new_child->pid = pid;
		new_child->next = NULL;
		end->next = new_child;
	}
	else
		debugger("end is null\n");
}

void	delete_last_child(t_msdata *data)
{
	t_childs	*end;

	end = get_second_last_child(data);
	if (end != NULL)
	{
		free(end->next);
		end->next = NULL;
	}
	else
	{
		end = get_last_child(data);
		if (end != NULL)
		{
			free(end);
			data->childs = NULL;
		}
	}
}

void	reset_childs(t_msdata *data)
{
	while (data->childs->next != NULL)
		delete_last_child(data);
}
