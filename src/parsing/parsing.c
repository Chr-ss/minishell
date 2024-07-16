/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:02:41 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/16 18:52:47 by crasche       ########   odam.nl         */
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
	if(single_q == false)
		return (-1);
	else if (double_q == false)
		return (-2);
	return(1);
}

static int	parsing_syntax_pipe(t_msdata *data)
{
	int		i;

	i = 0;
	i = skipspace(data->line, i);
	if (data->line[i] == '|')
		return (-1);
	return(0);
}

static int	parsing_syntax_whitespace(t_msdata *data)
{
	int	i;

	i = 0;
	i = skipspace(data->line, i);
	if (data->line[i] == '\0')
		return (-1);
	return (0);
}

int	parsing(t_msdata *data)
{
	if (!data->line || !data->line[0])
		return (-1);
	if (parsing_syntax_whitespace(data) == -1)
		return (-1);
	if(parsing_syntax_pipe(data) == -1)
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		data->exit_code = 2;
		return (-1);
	}
	if(parsing_syntax_quotes(data) == -1)
	{
		write(2, " unexpected EOF while looking for matching `''\n", 47);
		data->exit_code = 2;
		return (-1);
	}
	if(parsing_syntax_quotes(data) == -2)
	{
		write(2, " unexpected EOF while looking for matching `\"'\n", 48);
		data->exit_code = 2;
		return (-1);
	}
	return (0);
}
