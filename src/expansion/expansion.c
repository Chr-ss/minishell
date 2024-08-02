/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 16:31:20 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/02 13:48:40 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_exit_code(t_msdata *data, t_expand *exp, int *pos)
{
	char	*exit_code;

	(*pos)++;
	(*pos)++;
	exit_code = ft_itoa(data->exit_code);
	if (!exit_code)
		error("expand_exit_code: malloc error", data);
	exp->env = exit_code;
	expand_var_nl(data, exp);
	if (exit_code)
		free(exit_code);
	exp->env = NULL;
}

static char	*expand_getenv(char **envp, char *env_start, int env_len)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], env_start, env_len) == 0)
		{
			if (envp[i][env_len] == '=')
				return (&envp[i][env_len + 1]);
		}
		i++;
	}
	return (NULL);
}

static void	expand_var(t_msdata *data, t_expand *exp, int *pos)
{
	char	*env_start;
	int		env_len;

	(*pos)++;
	env_start = &data->line[*pos];
	env_len = 0;
	while (ft_isalnum((int) data->line[*pos]) || data->line[*pos] == '_')
	{
		(*pos)++;
		env_len++;
	}
	exp->env = expand_getenv(data->envp, env_start, env_len);
	expand_var_nl(data, exp);
}

static void	expand_copy(t_msdata *data, t_expand *exp)
{
	int		pos;
	bool	single_q;
	bool	double_q;

	single_q = true;
	double_q = true;
	pos = 0;
	while (data->line[pos])
	{
		expand_quote_check(data->line[pos], &single_q, &double_q);
		if (data->line[pos] == '$' && \
			ft_isdigit((int) data->line[pos + 1]) && single_q == true)
			pos += 2;
		else if (data->line[pos] == '$' && data->line[pos + 1] == '?' \
			&& single_q == true)
			expand_exit_code(data, exp, &pos);
		else if (data->line[pos] != '$' || single_q == false || \
			(data->line[pos] == '$' && !ft_isalpha((int) data->line[pos + 1]) &&
			data->line[pos + 1] != '_' && single_q == true))
		{
			expand_copy_chars(data, exp, &pos);
		}
		else if (data->line[pos] == '$' && single_q == true)
			expand_var(data, exp, &pos);
	}
}

char	*expand(t_msdata *data)
{
	t_expand	exp;

	expand_exp_init(data, &exp);
	exp.line = ft_dynstralloc(exp.line, &exp.capacity);
	if (!exp.line)
		error("expesion, malloc error.", data);
	expand_copy(data, &exp);
	if (data->line)
		free(data->line);
	data->line = NULL;
	data->exp = NULL;
	return (exp.line);
}
