/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/01 18:31:52 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

/**
 * @brief this function will prepare the child with dup and closing
 * all the correct fds
 * @param t_msdata *data 
 * @param t_cmd *cmd 
 * @return int, 1 is for file descriptor error, 0 is good execution 
 * @exception
 * if something goes wrong then error function is called
 */
int		execute_child_dup(t_msdata *data, t_cmd *cmd);

/**
 * @brief this function will execute the cmd in a child
 * @param t_msdata *data 
 * @param t_cmd *cmd 
 * @return void 
 */
void	execute_child(t_msdata *data, t_cmd *cmd);

/**
 * @brief this function will check if cmd is minishell and prepares
 * parent for this.
 * @param t_msdata *data 
 * @param t_cmd *cmd 
 * @return void 
 */
void	execute_child_minishell(t_msdata *data, t_cmd *cmd);

/**
 * @brief this function will check if cmd is a builtin, if function
 * is builtin it will execute and return exit code of builtin.
 * Otherwise it will return -1
 * @param t_msdata *data 
 * @param t_cmd *cmd 
 * @return int 
 */
int		execute_check_builtin(t_msdata *data, t_cmd *cmd);

/**
 * @brief this function will dup and close all fds for 
 * the execution of child
 * @param t_msdata *data 
 * @param t_cmd *cmd 
 * @exception if something goes wrong then error function will 
 * be called
 */
int		execute_child_dup_fd(t_msdata *data, t_cmd *cmd);

/**
 * @brief
 * This function takes the cmd, search for the path and adds it to argv
 * @param t_cmd **cmd_s
 * @param path_cmd **char
 * @return
 * Function returns int to indicate succesful operation
 * 0 is succesful operation
 * 1 is unsuccesful operation due to command not found
 * @exception
 *  -1 is unsuccesful operation due to malloc failure
*/
int		add_command_to_argv(t_cmd	**cmd_s, char	**path_cmd);

/**
 * @brief
 * This function takes the cmd, search for the path and returns the
 * path if it finds it
 * @param char	*cmd
 * @param t_msdata *data
 * @param t_cmd **path_cmd
 * @return
 * Function returns int to indicate succesful operation
 * 0 is succesful operation
 * 1 is unsuccesful operation due to command not found
 * @exception
 *  -1 is unsuccesful operation due to malloc failure
*/
int		execute_path(char	*cmd, t_msdata *data, char **path_cmd);

/**
 * @brief this function goes into waitpid to wait for the child processes 
 * @param int pid
 * @param int *wstatus
 * @param t_msdata *data
 * @return int 
 * @note 1 is continue waiting
 * @note 0 stop waitin
 */
int	execute_wait(int pid, int *wstatus, t_msdata* data);

#endif	// EXECUTION_H
