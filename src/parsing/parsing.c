/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:02:41 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/02 16:30:21 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parsing_syntax_quotes(t_msdata *data)
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

// int	parsing_syntax_meta(t_msdata *data)
// {
// 	int		i;

// 	i = 0;
// 	while(data->line[i])
// 	{
// 		i = skipspace(data->line, i);
// 		if (data->line[i] == '|' || data->line[i] == '' )
// 		i++;
// 	}
// 	return(1);
// }

void	parsing_syntax(t_msdata *data)
{
	if (!data->line)
		error("parsing_syntax, syntax error. (NULL line)");
	if(parsing_syntax_quotes(data) == -1)
		error("parsing_syntax, syntax error. (quotes)");
	// if(parsing_syntax_meta(data) == -1)
	// 	error("parsing_syntax, syntax error.");
}

void	parsing(t_msdata *data)
{
	//check for syntax errors before tokenizing
	// | ls  | cat |
	// "hello
	// >>> <<<
	parsing_syntax(data);
}
