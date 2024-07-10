/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/07/10 14:02:04 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

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
int	add_command_to_argv(t_cmd	**cmd_s, char	**path_cmd);

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
int	execute_path(char	*cmd, t_msdata *data, char **path_cmd);

#endif	// EXECUTION_H

