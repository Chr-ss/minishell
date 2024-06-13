/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_expansion.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 16:31:20 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/05 21:25:26 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_expand_exp_init(t_msdata *data, t_expend *exp)
{
	data->exp = exp;
	exp->env = NULL;
	exp->line = NULL;
	exp->line_pos = 0;
	exp->capacity = DYNSTRING;
}

void	ms_expand_var_nl(t_expend *exp)
{
	int	i;

	i = 0;
	while (exp->env && exp->env[i])
	{
		if (exp->line_pos == exp->capacity)
		{
			exp->line = ft_dynstralloc(exp->line, &exp->capacity);
			if(!exp->line)
				ms_error("ms_expesion, malloc error.");
		}
		exp->line[exp->line_pos] = exp->env[i];
		exp->line_pos++;
		i++;
	}
	exp->env = NULL;
}

char	*ms_expand_getenv(char **envp, char *env_start, int env_len)
{
	int	i;

	i = 0;
	while(envp[i])
	{
		if(ft_strncmp(envp[i], env_start, env_len) == 0)
			return (&envp[i][env_len + 1]);
		i++;
	}
	return (NULL);
}

void	ms_expand_var(t_msdata *data, t_expend *exp, int *pos)
{
	char	*env_start;
	int		env_len;

	env_start = &data->line[*pos];
	env_len = 0;
	while(ft_isalnum((int) data->line[*pos]) || data->line[*pos] == '_')
	{
		(*pos)++;
		env_len++;
	}
	exp->env = ms_expand_getenv(data->envp, env_start, env_len);
	ms_expand_var_nl(exp);
}

void	ms_expand_copy(t_msdata *data, t_expend *exp)
{
	int	pos;
	bool	single_q;
	bool	double_q;

	single_q = true;
	double_q = true;
	pos = 0;
	while (data->line[pos])
	{
		if(data->line[pos] == '\'' && double_q == true)
			single_q = !single_q;
		else if (data->line[pos] == '"' && single_q == true)
			double_q = !double_q;
		if (data->line[pos] != '$' || single_q == false)
		{
			if (exp->line_pos == exp->capacity)
			{
				exp->line = ft_dynstralloc(exp->line, &exp->capacity);
				if(!exp->line)
					ms_error("ms_expesion, malloc error.");
			}
			exp->line[exp->line_pos] = data->line[pos++];
			exp->line_pos++;
		}
		else if (data->line[pos] == '$' && ft_isdigit((int) data->line[pos + 1]) && single_q == true)
			pos += 2;
		else if (data->line[pos] == '$' && single_q == true)
		{
			pos++;
			ms_expand_var(data, exp, &pos);
		}
	}
}

char	*ms_expand(t_msdata *data)
{
	t_expend	exp;

	ms_expand_exp_init(data, &exp);
	exp.line = ft_dynstralloc(exp.line, &exp.capacity);
	if(!exp.line)
		ms_error("ms_expesion, malloc error.");
	ms_expand_copy(data, &exp);
	if (data->line)
		free(data->line);
	data->line = NULL;
	data->exp = NULL;
	return (exp.line);
}
