/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 15:58:39 by spenning          #+#    #+#             */
/*   Updated: 2024/06/28 17:29:11 by spenning         ###   ########.fr       */
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

void	echo_nl(void)
{
	ft_printf("\n");
}

void	echo(t_msdata *data)
{
	int		print_nl;
	int		arglen;
	char	*printline;

	print_nl = 1;
	arglen = double_array_len(data->argv);
	if (arglen == 1)
		return (echo_nl());
	printline = echo_getline(data->argv, &print_nl);
	if (printline == NULL)
		error("error in malloc for echo");
	ft_printf("%s", printline);
	if (print_nl)
		echo_nl();
}
