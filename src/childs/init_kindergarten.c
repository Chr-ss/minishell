/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_kindergarten.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/01 15:51:26 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/01 16:48:20 by spenning      ########   odam.nl         */
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

t_childs *get_last_child(t_msdata* data)
{
	t_childs *last;

	last = NULL;
	if (data->childs)
		last = data->childs;
	while (last->next != NULL)
		last = last->next;
	return (last);
}

t_childs *get_second_last_child(t_msdata* data)
{
	t_childs *last;

	last = NULL;
	if (data->childs->next)
		last = data->childs;
	while (last->next->next != NULL)
		last = last->next;
	return (last);
}

void	add_child(int	pid, t_msdata* data)
{
	t_childs	*new_child;
	t_childs	*end;

	new_child = ft_calloc(sizeof(t_childs), 1);
	if (new_child == NULL)
		error("add child: malloc error", data);
	end = get_last_child(data);
	if (end != NULL)
	{
		debugger("adding %d to child linked list\n", pid);
		new_child->pid = pid;
		new_child->next = NULL;
		end->next = new_child;
	}
	else
		debugger("lol NULL");
}

void	delete_last_child(t_msdata* data)
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

void	reset_childs(t_msdata* data)
{
	while(data->childs->next != NULL)
		delete_last_child(data);
}

void	print_childs(t_msdata* data)
{
	int			index;
	t_childs	*temp;

	index = 0;
	temp = data->childs;
	while(temp != NULL)
	{
		debugger("index %d child_pid %d\n", index, temp->pid);
		temp = temp->next;
		index++;
	}
}


void	kill_all_childs(t_msdata* data)
{
	t_childs *temp;
	
	temp = data->childs;
	while (temp != NULL)
	{
		kill(temp->pid, SIGKILL);
		temp = temp->next;
	}
}

