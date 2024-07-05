/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/05 19:11:52 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//exit ; echo $? = 0 
// exit 255+ is undefined

// TODO: ARGV should always be made eventhough no arguments given to command
int	mini_exit(char **argv)
{
	int		ec;

	ec = 0;
	if (argv)
	{
		if (double_array_len(argv) > 1)
			return (1);
	if (argv[0])
		ec = ft_atoi(argv[0]) % 256;
	}
	exit(ec);
}
