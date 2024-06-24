/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:32:33 by crasche           #+#    #+#             */
/*   Updated: 2024/06/24 19:14:06 by spenning         ###   ########.fr       */
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
# include <dirent.h>
# include <errno.h>
# include <sys/stat.h>

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
	t_cmd		*cmd_head;
// TOKEN TO CMD
	t_cmd		*cmd_curr;
	char		*line;
	int			pos;
// TOKEN TO CMD
	char		**argv;
	char		**envp;
	char		pwd[PATH_MAX];
	t_expend	*exp;
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



// BUILT-INS:

//[Description]
//This function is the built-in for cd
//[Parameters]
// char **args
// char **envp
//[Return]
//Function successful or not succesful operation based on int
// 0 == succesful operation
//[Error]
// 1 == unsuccesful operation
int cd (t_msdata *data);

//UTILS

//[Description]
//This function will search the envp member in the data structure
// for the envp as paramater. 
//[Parameters]
// t_msdata *data
// char *envp
//[Return]
//Function returns malloced string of the envp if envp was found.
//[Error]
//Will give back NULL on memory allocation or if envp was not found
//[Note]
//Return values and error code will be changed due to ambiguity
char	*get_envp(t_msdata *data, char *envp);

//[Description]
//This function will change a envp in the envp member with env based
//on the key passed. 
//e.g. change_envp("HOME", "/home/else", envp)
//will change the value of $HOME to "HOME=/home/else"
//[Parameters]
// char *key
// char *env
// char **envp
//[Return]
//Function returns int to indicate succesful operation or not
// 0 is a succesful operation
//[Error]
// 1 is a unsuccesful operation
int	change_envp(char *key, char *env, char **envp);

//[Description]
//This function will give back the index of the env
// in envp
//[Parameters]
// char *env
// char **envp
//[Return]
//Function returns int to index number or it will give back
// an error value
// >0 is index number
//[Error]
// -1 is env not found
int	get_envp_index(char *env, char **envp);

//[Description]
//This function is used to free a 2d char array, assuming the 2d array
// is null terminated.
//[Parameters]
// char **arr
//[Return]
//Function returns nothing
void	free_char_array(char **arr);

//[Description]
//This function returns lengt of 2d char array
//[Parameters]
// char **vector
//[Return]
//Function returns length of array.
int	double_array_len(char **vector);

//[Description]
//This function checks if a dir exists or not.
// function can take relative and absolute path
//[Parameters]
// char *dirname
//[Return]
//Function returns int
// 0 is not found
// 1 is found
// -1 is error
int	check_dir(char *dirname);


// FUNCTIONS:
void	ms_readline(t_msdata *data);

void	ms_parsing(t_msdata *data);
void	ms_error(char *msg);

// UTILS_CHRISS
char	*str_expand(char *str, int *capacity);
int		ms_skipspace(char *str, int pos);

// INITDATA.c
void	ms_initdata_cpy_envp(t_msdata *data, char **envp);
void	ms_initdata(t_msdata *data, char **argv, char **envp);

// PARSING
void	ms_parsing(t_msdata *data);
void	ms_parsing_syntax(t_msdata *data);
int		ms_parsing_syntax_quotes(t_msdata *data);

// EXPANSION
void	ms_expand_exp_init(t_msdata *data, t_expend *exp);
void	ms_expand_var_nl(t_expend *exp);
char	*ms_expand_getenv(char **envp, char *env_start, int length);
void	ms_expand_var(t_msdata *data, t_expend *exp, int *pos);
void	ms_expand_copy(t_msdata *data, t_expend *exp);
char	*ms_expand(t_msdata *data);

// SORT THIS LATER
void	ms_token_to_strarr(t_msdata *data, char **strarr, t_token token);
// void ms_clear_append(t_cmd *cmd);
void	ms_init_type_handler(t_token (*type_handler[8])(t_msdata *data, t_cmd *cmd, t_token token, int *pos));
t_token ms_token_to_cmd(t_msdata *data, t_token token, int *pos);
t_token ms_type_handler_append(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_eof(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_error(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_heredoc(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_pipe(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_rein(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_reout(t_msdata *data, t_cmd *cmd, t_token token, int *pos);
t_token ms_type_handler_word(t_msdata *data, t_cmd *cmd, t_token token, int *pos);

void	ms_unexpected_token(t_msdata *data, t_token token);
void printf_cmd(t_cmd *cmd);
char	**ms_extend_strarr(t_cmd *cmd, char **strarr, int strarr_size);
void ms_l(t_msdata *data, char *line);
void ms_readline(t_msdata *data);
int ms_strarr_size(char **strarr);


void	temp_print_tokens(t_msdata *data, char *line);
t_token	ms_tokenizer(char *line);
int	ft_isbashtoken(int c);

void	ms_openfile(t_cmd *cmd, t_token token, int open_flag, int *fd);

#endif	// MINISHELL_H
