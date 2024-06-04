/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_expansion.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 16:31:20 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/04 18:14:15 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ms_expansion_exp_init(t_msdata *data, t_expend *exp)
{
	data->exp = exp;
	exp->env = NULL;
	exp->line = NULL;
	exp->line_pos = 0;
	exp->capacity = DYNSTRING;
}

void	ms_expansion_var_nl(t_expend *exp)
{
	while (exp->env && *exp->env)
	{
		if (exp->line_pos == exp->capacity)
		{
			exp->line = ft_dynstralloc(exp->line, &exp->capacity);
			if(!exp->line)
				ms_error("ms_expesion, malloc error.");
		}
		exp->line[exp->line_pos] = *exp->env;
		exp->env++;
		exp->line_pos++;
	}
}

char	*ms_expansion_getenv(char **envp, char *env_start, int env_len)
{
	int	i;

	i = 0;
	while(envp[i])
	{
		printf("%s\n", envp[i]);
		if(ft_strncmp(envp[i], env_start, env_len))
			return (&envp[i][env_len + 1]);
		i++;
	}
	return (NULL);
}

void	ms_expansion_var(t_msdata *data, t_expend *exp, int *pos)
{
	char	*env_start;
	int		env_len;

	env_start = &data->line[*pos];
	env_len = 0;
	while(ft_isalnum((int) data->line[*pos]))
	{
		(*pos)++;
		env_len++;
	}
	exp->env = ms_expansion_getenv(data->envp, env_start, env_len); //you need to use find_env;
	ms_expansion_var_nl(exp);
}

void	ms_expansion_copy(t_msdata *data, t_expend *exp)
{
	int	pos;

	pos = 0;
	while (data->line[pos])
	{
		if (exp->line_pos == exp->capacity)
		{
			exp->line = ft_dynstralloc(exp->line, &exp->capacity);
			if(!exp->line)
				ms_error("ms_expesion, malloc error.");
		}
		if (data->line[pos] != '$')
		{
			exp->line[exp->line_pos] = data->line[pos++];
			exp->line_pos++;
		}
		else if (data->line[pos] == '$' && ft_isdigit((int) data->line[pos + 1]))
			pos += 2;
		else if (data->line[pos] == '$')
		{
			pos++;
			ms_expansion_var(data, exp, &pos);
		}
	}
}

char	*ms_expansion(t_msdata *data)
{
	t_expend	exp;

	ms_expansion_exp_init(data, &exp);
	exp.line = ft_dynstralloc(exp.line, &exp.capacity);
	if(!exp.line)
		ms_error("ms_expesion, malloc error.");
	ms_expansion_copy(data, &exp);
	if (data->line)
		free(data->line);
	data->line = NULL;
	data->exp = NULL;
	return (exp.line);
}
