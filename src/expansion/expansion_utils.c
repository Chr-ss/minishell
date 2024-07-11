/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 17:08:36 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 13:50:30 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_var_nl(t_msdata *data, t_expand *exp)
{
	int	i;

	i = 0;
	while (exp->env && exp->env[i])
	{
		if (exp->line_pos == exp->capacity)
		{
			exp->line = ft_dynstralloc(exp->line, &exp->capacity);
			if (!exp->line)
				error("expesion, malloc error.", data);
		}
		exp->line[exp->line_pos] = exp->env[i];
		exp->line_pos++;
		i++;
	}
	exp->env = NULL;
}

void	expand_exp_init(t_msdata *data, t_expand *exp)
{
	data->exp = exp;
	exp->env = NULL;
	exp->line = NULL;
	exp->line_pos = 0;
	exp->capacity = DYNSTRING;
}

void	expand_quote_check(char c, bool *single_q, bool *double_q)
{
	if (c == '\'' && *double_q == true)
		*single_q = !*single_q;
	else if (c == '"' && *single_q == true)
		*double_q = !*double_q;
}
