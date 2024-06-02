/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/02 13:51:37 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>

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
	char	**argv;
	char	**envp;
}	t_msdata;

// FUNCTIONS:
void	ms_parsing(t_msdata *data);
void	ms_init_cmdlist(t_cmd **cmd);
char	*ms_readline(t_msdata *data, char **argv);
void	ms_error(char *msg);

// UTILS_CHRISS
char	*str_expand(char *str, int *capacity);

// MS_INITDATA.c
void	ms_initdata_cpy_envp(t_msdata *data, char **envp);
void	ms_initdata(t_msdata *data, char **argv, char **envp);

#endif	// MINISHELL_H
