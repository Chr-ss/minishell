/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:29:43 by crasche           #+#    #+#             */
/*   Updated: 2024/06/18 16:59:34 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_msdata	data;

	(void)argc;
	// if (argc > 1)
	// 	ms_error("Invalid argument count.");
	ms_initdata(&data, argv, envp); // malloc >> data->cmd_head, data->envp
	cd(&data);
	ms_readline(&data);
	ms_error("No, error.");
	return (0);
}
