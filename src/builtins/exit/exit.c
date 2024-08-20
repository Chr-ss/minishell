/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/25 14:59:21 by spenning      #+#    #+#                 */
/*   Updated: 2024/08/20 17:57:50 by spenning      ########   odam.nl         */
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
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
		return (1);
	}
	if ((argv && argv[0][index] == '-') || (argv && argv[0][index] == '+'))
		index++;
	while (argv && argv[0][index])
	{
		if (!ft_isdigit(argv[0][index++]))
		{
			write(STDERR_FILENO, "exit: numeric argument required\n", 32);
			return (2);
		}
	}
	return (0);
}

int	mini_exit_long_long_error(void)
{
	write(STDERR_FILENO, "exit: numeric argument required\n", 32);
	return (2);
}

int	mini_exit_long_long_check(char *code)
{
	int	len;

	len = ft_strlen(code);
	if (len > 20)
		return (mini_exit_long_long_error());
	if (len == 20)
	{
		if ((code[0] == '-' && ft_strcmp(code, "-9223372036854775808") > 0) \
		|| (code[0] == '+' && ft_strcmp(code, "+9223372036854775807") > 0))
			return (mini_exit_long_long_error());
	}
	else
	{
		if (len > 19)
			return (mini_exit_long_long_error());
		else if (len == 19 && ft_strcmp(code, "9223372036854775807") > 0)
			return (mini_exit_long_long_error());
	}
	return (ft_atoi(code) % 256);
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
			ec = mini_exit_long_long_check(argv[0]);
	}
	else
		ec = code % 256;
	free_all(data);
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	exit(ec);
}
