/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:29:43 by crasche           #+#    #+#             */
/*   Updated: 2024/06/28 17:25:43 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	if (argc > 1)
		error("Invalid argument count.");
	initdata(&data, argv, envp); // malloc >> data->cmd_head, data->envp
	input_handling(&data);
	error("No, error.");
	return (0);
}
