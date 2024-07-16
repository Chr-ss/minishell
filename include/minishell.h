/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/16 15:15:36 by spenning      ########   odam.nl         */
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

# ifndef DEBUG
#  define DEBUG 0
# endif

enum e_pipe
{
	RD,
	WR
}	;

typedef struct s_cmd
{
	char			*cmd;		// can be used for execution
	char			**argv;		// can be used for execution
	char			**heredoc;	// heredoc delimiters
	struct s_cmd	*pipe;		// if NULL no more pipe
	int				infd;		// if -1 do not execute cmd | if 0 no change
	int				outfd;		// if -1 do not execute cmd | if 0 no change
	int				pipefd[2];	// holds the pipe from previous cmd to current
}	t_cmd;

typedef struct s_msdata
{
	t_cmd		*cmd_head;		// head of CMD struct
	t_cmd		*cmd_curr;
	char		*line;
	int			pos;
	char		**envp;
	int			exit_code;
	char		pwd[PATH_MAX];
	t_expand	*exp;			// struct for line expansion
	int			org_stdout;
}	t_msdata;

// SIGNAL:

/**
 * @brief
 * This function initializes the signal handlers for this
 *  program
 * @param
 *  void
 * @return
 *  void
 * @exception
 *  exit (EXIT_FAILURE)
*/
void		init_signal(void);

// INITDATA.c

/**
 * @brief
 * This function initializes the data strcut in the main function.
 * @param
 *  t_msdata *data
 *  char **envp
 * @return
 *  void
 * @exception
 *  exit (EXIT_FAILURE)
*/
void		initdata(t_msdata *data, char **envp);

// BUILT-INS:

/**
 * @brief This function is the built-in for cd
 * @param t_msdata *data
 * @param char **argv
 * @return 0 is succesful operation
 * @exception 1 is unsuccesful operation
*/
int			cd(t_msdata *data, char **argv);

/**
 * @brief
 * This function is the built-in for pwd
 * @param
 *  t_msdata *data
 * @return
 * Function returns nothing
*/
int	pwd(void);

/**
 * @brief
 * function is the built-in for env
 * it will print all environment variables to stdout with
 * @param t_msdata *data
 * @param char **argv
 * @return
 * Function returns int to indicate operation successful or not,
 * 0 is succesful operation
 * 1 is unsuccesful operation due to envp not found
*/
int	env(t_msdata *data, char **argv);

/**
 * @brief
 * This function is the built-in for exit.
 *  It will exit with the exit code provided
 *  as argument with argv, the exit code should be in between
 *  0-255, otherwise it will returned a remaining exit code
 *  after modulu operation.
 * @param t_msdata *data
 * @param char **argv
 * @return
 * Function returns nothing
*/
int			mini_exit(t_msdata *data, char **argv);

/**
 * @brief
 * This function is the built-in for export.
 * It will take the argument, and add it to the environment variables
 * the expected argument is name[=word]
 * @note
 * export [] will return env command output
 * @note
 * export name[] will add name with empty string as [=word]
 * @param t_msdata* data
 * @param char** argv
 * @return
 * Function returns nothing
*/
int	export(t_msdata *data, char **argv);

/**
 * @brief
 * This function is the built-in function for unset. It will unset
 * a environmental variable if it exists.
 * @param t_msdata* data
 * @param char** argv
 * @param char* arg
 * @note
 * 	the char *arg is a temporary parameter to work with export builtin
 * 	pass NULL if you want to use with argv in data struct, otherwise arg
 * 	will be used
 * @return
 * Function returns int to indicate operation successful or not,
 * 0 is succesful operation
 * 1 is unsuccesful operation due to envp not found
 * @exception
 * -1 is unsuccesful operation due to malloc error
 * exit
*/
int			unset(t_msdata *data, char **argv, char *arg);

/**
 * @brief
 * This function is the built-in for echo.
 * it will take the argv and print it with a nl,
 * expect if -n is specified
 * @param t_msdata *data
 * @param char **argv
 * @return Function returns int to indicate exit code
 * @exception Will call error() when malloc error occurs
*/
int			echo(t_msdata *data, char **argv);

// UTILS:

/**
 * @brief
 * This function will take the same arguments as printf
 * and then print out if debug flag is active DEBUG=1
 * @param char *format
 * @param (...) variadic parameters
 * @return
 * Function returns nothing
*/
void		debugger(char *format, ...);

/**
 * @brief
 * will free the whole command structures
 * @param t_cmd *cmd
 * @return
 * Function returns nothing
*/
void	free_cmd(t_cmd *cmd);

/**
 * @brief
 * This function will free everything except the cmd structure in the 
 * data structure
 * @param t_msdata *data
 * @return
 * Function returns nothing
*/
void	free_data(t_msdata *data);

/**
 * @brief
 * This function will free everything in the data structure
 * @param t_msdata *data
 * @return
 * Function returns nothing
*/
void	free_all(t_msdata *data);


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
 * @return
 * Function returns int to indicate succesful operation 
 * 0 is succesful operation
 * @exception
 * 1 is unsuccesful operation due to malloc error
*/
int		copy_over_str(int dst_index, int src_index, char**dst, char **src);

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
char		*get_envp_key(char *input);

/**
 * @brief
 * This function takes a full envp, and it will retrieve the value of key[=value]
 * of a envp.
 * @param
 *  char** input
 * @exception
 * Function will return null if there is a malloc error
 * @return
 * Function will return a malloced string.
 * If there was no value in the envp it will
 * return an empty string ""
*/
char		*get_envp_value(char *input);

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
void		swap_envp(t_msdata *data, char **envp);

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
void		add_envp(t_msdata *data, char *key, char *value);

/**
 * @brief
 * This function will search the envp member in the data structure
 *  for the envp as paramater.
 * @param
 *  t_msdata* data
 * @param
 *  char* envp
 * @return
 * Function returns int to indicate succesful operation or not
 * 0 is succesful operation
 * 1 is unsuccesful operation due to parameter not found
 * @exception
 * -1 is unsuccesful operation due to malloc failure
*/
int			get_envp(t_msdata *data, char *envp, char **env);

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
int			change_envp(char *key, char *env, char **envp);

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
int			get_envp_index(char *env, char **envp);

/**
 * @brief
 * This function is used to free a 2d char array, assuming the 2d array
 *  is null terminated.
 * @param
 *  char **arr
 * @return
 * Function returns nothing
*/
void		free_char_array(char **arr);

/**
 * @brief
 * This function returns lengt of 2d char array
 * @param
 *  char **vector
 * @return
 * Function returns length of array.
*/
int			double_array_len(char **vector);

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
int			check_dir(char *dirname);

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
int			check_file(char *file);

/**
 * @brief
 * This function is using the current position and
 *  moving it until there is not more whitespace.
 * @param
 *  char *str
 *  int pos
 * @return
 *  new position integer
*/
int			skipspace(char *str, int pos);

/**
 * @brief
 * This function checks if a char is a bash token.
 * @param
 *  char c
 * @return
 *  0 if no
 *  1 if yes
*/
int			ft_isbashtoken(int c);

/**
 * @brief
 * This function is extending **char by 1.
 * @param
 *  char **strarr
 *  int strarr_size
 * @return
 *  **char with +1 size
 * @exception
 *  NULL
*/
char		**extend_strarr(char **strarr, int strarr_size);

// INPUT HANDLING:

/**
 * @brief
 * This function is using readline to accept
 *  commands from the user.
 * @param
 *  t_msdata *data
 * @return
 *  void
*/
void		input_handling(t_msdata *data);

/**
 * @brief
 * This function is parsing user input for syntax errors.
 * @param
 *  t_msdata *data
 * @return
 *  0
 * @exception
 *  -1
*/
int			parsing(t_msdata *data);

/**
 * @brief
 * Checks function takes the lines and converts it
 *  into tokens. The tokens then are used to fill
 *  the cmd struct.
 * @param
 *  t_msdata *data
 *  char *line
 * @return
 *  0
 * @exception
 *  -1
*/
int			line_to_token(t_msdata *data, char *line);

/**
 * @brief
 * Checks for heredoc delimitors in all cmd structs.
 *  Found delimitors activate readline and read
 *  into a pipe for each cmd struct and safe pipe
 *  readfd in infd for each strcut.
 * @param
 *  t_msdata *data
 * @return
 *  0
 * @exception
 *  -1
*/
int			heredoc(t_msdata *data);

// EXECUTION:
void		execute(t_msdata *data);

// CMD RESET:
/**
 * @brief
 * This function is freeing all elements in cmd linked list.
 * @param
 *  t_msdata *data
 * @return
 *  void
*/
void		cmd_clear(t_msdata *data);

/**
 * @brief
 * This function is using cmd_clear() to clear the cmd struct
 *  and callocs a new clean first element.
 * @param
 *  t_msdata *data
 * @return
 *  0
 * @exception
 *  exit(EXIT_FAILURE) over error
*/
void		cmd_reset(t_msdata *data);

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
char		*expand(t_msdata *data);
void		expand_exp_init(t_msdata *data, t_expand *exp);
void		expand_var_nl(t_msdata *data, t_expand *exp);
void		expand_quote_check(char c, bool *single_q, bool *double_q);

// SORT THIS LATER
void		token_to_strarr(t_msdata *data, char **strarr, t_token token);
t_token		unexpected_token(t_msdata *data, t_token token);

t_token		tokenizer(char *line);
void		printf_cmd(t_cmd *cmd);
void		init_type_handler(t_token (*type_handler[8])(t_msdata *data, \
				t_cmd *cmd, t_token token, int *pos));
t_token		token_to_cmd(t_msdata *data, t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_append(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_eof(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_error(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_heredoc(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_pipe(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_rein(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_reout(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

/**
 * @brief
 * This function handles specified token type.
 * @param
 *  t_msdata *data
 *  t_cmd *cmd
 *  t_token token
 *  int *pos
 * @return
 *  t_token token
 * @exception
 *  token.type == TOKEN_ERROR
*/
t_token		type_handler_word(t_msdata *data, t_cmd *cmd, \
				t_token token, int *pos);

void		openfile(t_msdata *data, t_token token, int open_flag, int *fd);

/**
 * @brief
 *  This function checks if STDIN, STDOUT and
 *  STDERR are tty.
 * @param
 *  void
 * @return
 *  void
 * @exception
 *  exit(EXIT_FAILURE)
*/
void		check_tty(void);

/**
 * @brief
 *  Error function to exit minishell
 *  printf msg to STDERR and exit(EXIT_FAILURE)
 * @param
 *  char *msg
 *  t_msdata *data
 * @return
 *  void
 * @exception
 *  exit(EXIT_FAILURE)
*/
void		error(char *msg, t_msdata *data);

#endif	// MINISHELL_H
