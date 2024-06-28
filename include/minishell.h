/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/27 15:29:29 by crasche       ########   odam.nl         */
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

# include <signal.h>

# include <fcntl.h>

# include "../libft/include/libft.h"
# include "token.h"
# include "expansion.h"

# define DYNSTRING 8

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_cmd
{
	char			*cmd;		// can be used for execution
	char			**argv;		// can be used for execution
	char			**heredoc;	// heredoc delimiters (write heredocs into tempfile and transfer fd to infd??)
	struct s_cmd	*pipe;		// if NULL no more pipe
	int				infd;		// if -1 do not execute cmd | if 0 no change
	int				outfd;		// if -1 do not execute cmd | if 0 no change
}	t_cmd;

typedef struct s_msdata
{
	t_cmd		*cmd_head;		// head of CMD struct
// TOKEN TO CMD
	t_cmd		*cmd_curr;
	char		*line;
	int			pos;
// TOKEN TO CMD
	char		**argv;
	char		**envp;
	int			exit_code;
	char		pwd[PATH_MAX];
	t_expend	*exp;			// struct for line expansion
}	t_msdata;

// SIGNAL:

//[Description]
//This function initializes the signal handlers for this
// program
//[Parameters]
// void
//[Return]
//Function returns nothing
//[Error]
// if initialization goes wrong then exits with failure exit code
void	init_signal();

// FUNCTIONS:
void	input_handling(t_msdata *data);
void	parsing(t_msdata *data);
void	error(char *msg);

// UTILS
int		skipspace(char *str, int pos);
int		ft_isbashtoken(int c);
int		strarr_size(char **strarr);
char	**extend_strarr(t_cmd *cmd, char **strarr, int strarr_size);

// INITDATA.c
void	initdata_cpy_envp(t_msdata *data, char **envp);
void	initdata(t_msdata *data, char **argv, char **envp);

// PARSING
void	parsing(t_msdata *data);
void	parsing_syntax(t_msdata *data);
int		parsing_syntax_quotes(t_msdata *data);

// EXPANSION
void	expand_exp_init(t_msdata *data, t_expend *exp);
void	expand_var_nl(t_expend *exp);
char	*expand_getenv(char **envp, char *env_start, int length);
void	expand_var(t_msdata *data, t_expend *exp, int *pos);
void	expand_copy(t_msdata *data, t_expend *exp);
char	*expand(t_msdata *data);

// SORT THIS LATER
void	token_to_strarr(t_msdata *data, char **strarr, t_token token);
void	unexpected_token(t_msdata *data, t_token token);

t_token	tokenizer(char *line);
void	temp_print_tokens(t_msdata *data, char *line);
void	printf_cmd(t_cmd *cmd);
void	init_type_handler(t_token (*type_handler[8])(t_msdata *data, t_cmd *cmd, t_token token, int *pos));
t_token token_to_cmd(t_msdata *data, t_token token, int *pos);
t_token	type_handler_append(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_eof(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_error(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_heredoc(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_pipe(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_rein(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_reout(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token	type_handler_word(t_msdata *data, t_cmd *cmd, t_token token, int *pos);

void	openfile(t_cmd *cmd, t_token token, int open_flag, int *fd);

#endif	// MINISHELL_H
