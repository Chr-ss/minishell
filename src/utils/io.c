/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:41:05 by spenning          #+#    #+#             */
/*   Updated: 2024/06/24 18:53:06 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_dir(char *dirname)
{
	struct stat	s;
	int			err;

	err = stat(dirname, &s);
	if (err == -1)
	{
		if (ENOENT == errno)
			return (EXIT_FAILURE);
		else
			return (-1);
	}
	else
	{
		if (S_ISDIR(s.st_mode))
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
