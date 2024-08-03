/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/03 11:18:08 by mynodeus      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// TESTCASES
//exit ; echo $? = 0 
// exit 255+ is undefined

int	mini_exit_error(char **argv)
{
	int	index;

	index = 0;
	if (double_array_len(argv) > 1)
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	if ((argv && argv[0][index] == '-') || (argv && argv[0][index] == '+'))
		index++;
	while (argv && argv[0][index])
	{
		if (!ft_isdigit(argv[0][index++]))
		{
			write(2, " numeric argument required\n", 27);
			return (2);
		}
	}
	return (0);
}

int	mini_exit(t_msdata *data, char **argv, int code)
{
	int		ec;
	int		ret;

	ret = 0;
	ec = 0;
	if (argv)
	{
		ret = mini_exit_error(argv);
		if (ret)
			return (ret);
		if (argv[0])
			ec = ft_atoi(argv[0]) % 256;
	}
	else
		ec = code % 256;
	free_all(data);
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	exit(ec);
}
