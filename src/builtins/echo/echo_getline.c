/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo_getline.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 17:06:50 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/24 17:03:52 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/builtins.h"

char	*echo_getline_malloc(char *ret, char *arg)
{
	char	*temp;

	if (ret != NULL)
	{
		temp = ft_strjoin(ret, " ");
		if (temp == NULL)
		{
			if (ret)
				free(ret);
			return (NULL);
		}
		free(ret);
	}
	ret = ft_strjoin(temp, arg);
	if (ret == NULL)
	{
		if (temp)
			free(temp);
		return (NULL);
	}
	free(temp);
	return (ret);
}

char	*echo_getline_first_arg(char **argv, int *print_nl, int *i, int l)
{
	char	*ret;

	ret = ft_strdup(argv[*i]);
	if (ret == NULL)
		return (NULL);
	++(*i);
	while (!echo_check_nl(ret) && l > 1)
	{
		free(ret);
		*print_nl = 0;
		ret = ft_strdup(argv[*i]);
		if (ret == NULL)
			return (NULL);
		++(*i);
	}
	return (ret);
}

char	*echo_getline(char **argv, int *print_nl, int arglen)
{
	char	*ret;
	int		index;

	index = 0;
	ret = echo_getline_first_arg(argv, print_nl, &index, arglen);
	if (ret == NULL)
		return (NULL);
	while (argv[index] != NULL)
	{
		if (echo_check_nl(argv[index]))
		{
			ret = echo_getline_malloc(ret, argv[index]);
			if (ret == NULL)
				return (NULL);
		}
		else
			*print_nl = 0;
		index++;
	}
	return (ret);
}
