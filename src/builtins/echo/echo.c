/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 15:58:39 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/05 18:24:12 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

// TESTCASES
// echo -n lol lol
// echo lol echo lol
// echo
// echo -n
// echo -nnnnn
// echo -nnnn -nnnnn
// echo -nnnn nnnnn
// echo ----nnnn nnnnn
// echo ----nnnn loool -nnnnn

int	echo_check_nl(char *arg)
{
	int	index;
	int	ret;

	index = 0;
	ret = 1;
	if (arg[index] == '-')
		index++;
	while (arg[index] != '\0')
	{
		if (arg[index] != 'n')
			return (ret);
		index++;
	}
	if (index > 1)
		ret = 0;
	return (ret);
}

int	echo_nl(void)
{
	ft_printf("\n");
	return (0);
}

int	echo(char	**argv)
{
	int		print_nl;
	int		arglen;
	char	*printline;

	print_nl = 1;
	arglen = double_array_len(argv);
	if (arglen == 0)
		return (echo_nl());
	printline = echo_getline(argv, &print_nl);
	if (printline == NULL)
		error("error in malloc for echo");
	ft_printf("%s", printline);
	if (print_nl)
		echo_nl();
	return (0);
}
