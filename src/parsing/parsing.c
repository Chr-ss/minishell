/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:02:41 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 14:23:52 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	parsing_syntax_quotes(t_msdata *data)
{
	int		i;
	bool	single_q;
	bool	double_q;

	i = 0;
	single_q = true;
	double_q = true;
	while(data->line[i])
	{
		if(data->line[i] == '\'' && double_q == true)
			single_q = !single_q;
		else if (data->line[i] == '"' && single_q == true)
			double_q = !double_q;
		i++;
	}
	if(single_q == false || double_q == false)
		return (-1);
	return(1);
}

static int	parsing_syntax_meta(t_msdata *data)
{
	int		i;

	i = 0;
	while(data->line[i])
	{
		i = skipspace(data->line, i);
		if (data->line[i] == '|')
			return (-1);
		i++;
	}
	return(1);
}

int	parsing(t_msdata *data)
{
	if (!data->line)
	{
		perror("parsing_syntax, syntax error. (NULL line)");
		return (-1);
	}
	if(parsing_syntax_quotes(data) == -1)
	{
		perror("parsing_syntax, syntax error. (quotes)");
		return (-1);
	}
	if(parsing_syntax_meta(data) == -1)
	{
		perror("parsing_syntax, syntax error.");
		return (-1);
	}
	return (0);
}
