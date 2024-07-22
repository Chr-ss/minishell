/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/22 17:44:17 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//exit ; echo $? = 0 
// exit 255+ is undefined

// TODO: ARGV should always be made eventhough no arguments given to command
int	mini_exit(t_msdata *data, char **argv, int code)
{
	int		ec;

	ec = 0;
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	if (argv)
	{
		if (double_array_len(argv) > 1)
		{
			write(2, "exit: too many arguments\n", 25);
			return (1);
		}
		if (argv[0])
			ec = ft_atoi(argv[0]) % 256;
	}
	else
		ec = code % 256;
	free_all(data);
	exit(ec);
}
