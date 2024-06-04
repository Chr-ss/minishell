

typedef struct s_expend
{
	char			*start;
	int				length;
	char			*var;
	int				var_len;
	int				line_chars;
}	t_expend;

typedef struct s_linedata
{
	char			*line;
	int				line_pos;
	char			*exp_line;
	int				exp_pos;
	int				exp_len;
}	t_linedata;


void	ms_expansion_getlen(t_expend *exp_var)
{
	char	*str;

	str = ft_strndup(exp_var->start, exp_var->length);
	if (!str)
		ms_error("ms_expansion_getlen, alloc error.");
	exp_var->var = getenv(str);
	free(str);
}

void	ms_expansion_data_init(t_linedata *data, char *line)
{
	data->line = line;
	data->line_pos = 0;
	data->exp_line = NULL;
	data->exp_pos = 0;
	data->exp_len = ft_strlen(line);
}

void	ms_expansion_var_init(t_expend *exp_var)
{
	exp_var->start = NULL;
	exp_var->length = 0;
	exp_var->var = NULL;
	exp_var->var_len = 0;
	exp_var->line_chars = 0;
}

void	ms_expansion_loopline(t_linedata *data);

void	ms_expansion_var(t_linedata *data, t_expend *exp_var)
{

	exp_var->start = &data->line[data->line_pos];
	exp_var->length = 0;
	while (data->line[data->line_pos] && data->line[data->line_pos] != '$' && ft_isalnum((int) data->line[data->line_pos]))
	{
		data->line_pos++;
		exp_var->length++;
	}
	ms_expansion_getlen(exp_var);
	if (exp_var->var)
		exp_var->var_len = ft_strlen(exp_var->var);
	else
		exp_var->var_len = 0;
	data->exp_len += exp_var->var_len - exp_var->length;
	ms_expansion_loopline(data);
}

void	ms_expansion_copy(t_linedata *data, t_expend exp_var)
{
	if (!data->exp_line)
		data->exp_line = ft_calloc(data->exp_len + 1, sizeof(char));
	if (!data->exp_line)
			ms_error("ms_expansion_copy malloc error.");
	ft_memset(data->exp_line, '1', data->exp_len);
	if (exp_var.var)
		printf("\n%zu\n", ft_strlen(exp_var.var));
	while (exp_var.line_chars)
	{
		data->line_pos--;
		data->exp_len--;
		data->exp_line[data->exp_len] = data->line[data->line_pos];
		exp_var.line_chars--;
	}
	while (exp_var.var_len)
	{
		data->exp_len--;
		data->exp_line[data->exp_len] = exp_var.var[exp_var.var_len];
		exp_var.var_len--;
	}
	while (exp_var.length)
	{
		data->line_pos--;
		exp_var.length--;
		if (!exp_var.length)
			data->line_pos--;
	}
	printf("\n\nexp>>>%s\n\n", data->exp_line);
}

void	ms_expansion_loopline(t_linedata *data)
{
	t_expend	exp_var;

	ms_expansion_var_init(&exp_var);
	while(data->line[data->line_pos])
	{
		if (data->line[data->line_pos] == '$')
		{
			data->line_pos++;
			if (ft_isalpha((int) data->line[data->line_pos])) //data->line[data->line_pos] != '$' &&
			{
				ms_expansion_var(data, &exp_var);
				break ;
			}
			else if (ft_isdigit((int) data->line[data->line_pos]))
			{
				exp_var.length = 1;
				break ;
			}
			else if (data->line[data->line_pos] == '?')
				break ;
			else
				exp_var.line_chars++;
		}
		else
		{
			data->line_pos++;
			exp_var.line_chars++;
		}
	}
	ms_expansion_copy(data, exp_var);
}

char *ms_expesion(char **argv, char **line)
{
	t_linedata	data;

	(void) argv;
	ms_expansion_data_init(&data, *line);
	ms_expansion_loopline(&data);
	free(*line);
	*line = NULL;
	return (data.exp_line);
}
