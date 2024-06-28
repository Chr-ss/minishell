/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:59:21 by spenning          #+#    #+#             */
/*   Updated: 2024/06/26 15:15:43 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//exit ; echo $? = 0 
// exit 255+ is undefined

void	mini_exit(t_msdata *data)
{
	int		ec;

	ec = 0;
	if (double_array_len(data->argv) > 2)
		return ;
	if (data->argv[1])
		ec = ft_atoi(data->argv[1]) % 256;
	exit(ec);
}
