/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/04 14:27:25 by crasche       ########   odam.nl         */
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
# include <unistd.h>

# include <signal.h>

# include <fcntl.h>

# include "../libft/include/libft.h"
# include "token.h"
# include "expansion.h"
# include <dirent.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define DYNSTRING 8

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

enum e_pipe
{
	RD, WR
}	;

typedef struct s_cmd
{
	char			*cmd;		// can be used for execution
	char			**argv;		// can be used for execution
	char			**heredoc;	// heredoc delimiters (write heredocs into tempfile and transfer fd to infd??)
	struct s_cmd	*pipe;		// if NULL no more pipe
	int				infd;		// if -1 do not execute cmd | if 0 no change
	int				outfd;		// if -1 do not execute cmd | if 0 no change
	int 			pipefd[2];	// holds the pipe from previous cmd to current
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
	t_expand	*exp;			// struct for line expansion
}	t_msdata;

// SIGNAL:

/**
 * @brief
 * This function initializes the signal handlers for this
 *  program
 * @param
 *  void
 * @return
 * Function returns nothing
 * @exception
 *  if initialization goes wrong then exits with failure exit code
*/
void	init_signal();

// BUILT-INS:

/**
 * @brief
 * This function is the built-in for cd
 * @param
 *  t_msdata *data
 * @return
 *  0 is succesful operation
 * @exception
 *  1 is unsuccesful operation
*/
int cd (t_msdata *data);

/**
 * @brief
 * This function is the built-in for pwd
 * @param
 *  t_msdata *data
 * @return
 * Function returns nothing
*/
void	pwd(t_msdata *data);

/**
 * @brief
 * This function is the built-in for env
 * it will print all environment variables to stdout with
 * @param
 *  t_msdata *data
 * @return
 * Function returns nothing
*/
void	env(t_msdata *data);

/**
 * @brief
 * This function is the built-in for exit.
 *  It will exit with the exit code provided
 *  as argument with argv, the exit code should be in between
 *  0-255, otherwise it will returned a remaining exit code
 *  after modulu operation.
 * @param
 *  t_msdata* data
 * @return
 * Function returns nothing
*/
void	mini_exit(t_msdata *data);

/**
 * @brief
 * This function is the built-in for export.
 * It will take the argument, and add it to the environment variables
 * the expected argument is name[=word]
 * @note
 * export [] will return env command output
 * @note
 * export name[] will add name with empty string as [=word]
 * @param
 *  t_msdata* data
 * @return
 * Function returns nothing
*/
void	export(t_msdata *data);

/**
 * @brief
 * This function is the built-in function for unset. It will unset
 * a environmental variable if it exists.
 * @param
 *  t_msdata* data
 * @param
 *  char* arg
 * @note
 * 	the char *arg is a temporary parameter to work with export builtin
 * 	pass NULL if you want to use with argv in data struct, otherwise arg
 * 	will be used
 * @return
 * Function returns nothing
 * @exception
 * function will do nothing if there is an error with the parameters or
 * if variable is not found. If there is a malloc error then minishell will
 * exit
*/
void unset(t_msdata *data, char *arg);


/**
 * @brief
 * This function is the built-in for echo.
 * it will take the argv and print it with a nl,
 * expect if -n is specified
 * @param
 *  t_msdata* data
 * @return
 * Function returns nothing
*/
void	echo(t_msdata* data);

//UTILS

/**
 * @brief
 * This function will take an index and copy over the
 * string located at index from src to dst by mallocing a copy.
 * @param
 *  int index
 * @param
 *  char** dst
 * @param
 *  char** src
 * @exception
 * Function will free dst if there is a malloc error
 * @return
 * Function returns nothing
*/
void	copy_over_str(int dst_index, int src_index, char**dst, char **src);

/**
 * @brief
 * This function takes a full envp, and it will retrieve the key of key[=value]
 * of a envp.
 * @param
 *  char** input
 * @exception
 * Function will return null if there is a malloc error
 * @return
 * Function will return a malloced string.
*/
char	*get_envp_key(char *input);

/**
 * @brief
 * This function takes a full envp, and it will retrieve the value of key[=value]
 * of a envp.
 * @param
 *  char** input
 * @exception
 * Function will return null if there is a malloc error
 * @return
 * Function will return a malloced string. If there was no value in the envp it will
 * return an empty string ""
*/
char	*get_envp_value(char *input);


/**
 * @brief
 * This function will swap the current data->envp for a new set
 * of envp. data->envp will be freed
 * @param
 *  t_msdata* data
 * @param
 *  char** envp
 * @return
 * Function returns nothing
*/
void	swap_envp(t_msdata *data, char **envp);

/**
 * @brief
 * This function will add an env to envp, the expected arguments for
 * environment variables is is name[=word] which are translated to key[=value] as
 * parameters for this function
 * @param
 *  t_msdata* data
 * @param
 *  char* key
 * @param
 *  char* value
 * @return
 * Function returns nothing
*/
void	add_envp(t_msdata *data, char *key, char *value);

/**
 * @brief
 * This function will search the envp member in the data structure
 *  for the envp as paramater.
 * @param
 *  t_msdata* data
 * @param
 *  char* envp
 * @return
 * Function returns malloced string of the envp if envp was found.
 * @exception
 * Will give back NULL on memory allocation or if envp was not found
 * @note
 * Return values and error code will be changed due to ambiguity in future development
*/
char	*get_envp(t_msdata *data, char *envp);

/**
 * @brief This function will change a envp in the envp member
 * with env based on the key passed.
 * @note change_envp("HOME", "/home/else", envp)
will change the value of $HOME to "HOME=/home/else"
 * @param char* key
 * @param char* env
 * @param char** envp
 * @return 0 is a succesful operation
 * @exception 1 is a unsuccesful operation
 */
int	change_envp(char *key, char *env, char **envp);

/**
 * @brief
 * This function will give back the index of the env
 *  in envp
 * @param
 *  char *env
 * @param
 *  char **envp
 * @return
 *  >0 is index number
 * @exception
 *  -1 is env not found
*/
int	get_envp_index(char *env, char **envp);

/**
 * @brief
 * This function is used to free a 2d char array, assuming the 2d array
 *  is null terminated.
 * @param
 *  char **arr
 * @return
 * Function returns nothing
*/
void	free_char_array(char **arr);

/**
 * @brief
 * This function returns lengt of 2d char array
 * @param
 *  char **vector
 * @return
 * Function returns length of array.
*/
int	double_array_len(char **vector);

/**
 * @brief
 * This function checks if a dir exists or not.
 *  function can take relative and absolute path
 * @param
 *  char *dirname
 * @return
 *  0 is not found;
 *  1 is found
 * @exception
 *  -1 is error
*/
int	check_dir(char *dirname);


/**
 * @brief
 * This function checks if a file exists or not.
 *  function can take relative and absolute path
 * @param
 *  char *file
 * @return
 *  0 is not found;
 *  1 is found
 * @exception
 *  -1 is error
*/
int	check_file(char *file);


// FUNCTIONS:
void	input_handling(t_msdata *data);
void	parsing(t_msdata *data);
void	error(char *msg);

// EXECUTION:

void	execute(t_msdata *data);

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


void	clearcmd(t_msdata *data);

/**
 * @brief
 * This function checks for expansions.
 * Found expansons are expanded with subfunctions
 * @param
 *  t_msdata *data
 * @return
 *  char* expanded line
 * @exception
 *  only as error()
*/
char	*expand(t_msdata *data);
// static functions:
// void	expand_exp_init(t_msdata *data, t_expand *exp);
// void	expand_var_nl(t_expand *exp);
// char	*expand_getenv(char **envp, char *env_start, int length);
// void	expand_var(t_msdata *data, t_expand *exp, int *pos);
// void	expand_copy(t_msdata *data, t_expand *exp);


/**
 * @brief
 * Checks for heredoc delimitors in all cmd structs.
 * Found delimitors activate readline and read
 * into a pipe for each cmd struct with subfunctions.
 * @param
 *  t_msdata *data
 * @return
 *  void
 * @exception
 *  only as error()
*/
void	heredoc(t_msdata *data);

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
