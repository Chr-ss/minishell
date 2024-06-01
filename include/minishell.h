/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/01 18:55:42 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <stdlib.h>
# include <stdbool.h>

# include "../libft/include/libft.h"
# include "lexer.h"

# define DYNSTRING 8

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	char			*in;
	char			**out;
	struct s_cmd	*pipe;
	char			*here_doc;
	bool			append;
}	t_cmd;

typedef struct s_msdata
{
	t_cmd	*cmd;
}	t_msdata;

// FUNCTIONS:
void	ms_parsing(t_msdata *data);
void	ms_init_cmdlist(t_cmd **cmd);
char	*ms_readline(t_msdata *data, char **argv);
void	ms_error(char *msg);

// UTILS_CHRISS
int	str_double_size(char **str, int *capacity);
int	str_expand(char **str, int *capacity);
char	*str_expand_new(char *str, int *capacity);

#endif	// MINISHELL_H
