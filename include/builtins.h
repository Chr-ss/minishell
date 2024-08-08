/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/08/08 21:08:54 by spenning      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

/**
 * @brief this function is responsible to print the env
 * in export
 * @param t_msdata *data
 * @return int
 */
int		export_print_env(t_msdata *data);

/**
 * @brief
 * This function parses the directory argument passed for cd
 * @param t_msdata *data
 * @return
 * Function returns malloced char * of parsed path to directory
 * @exception
 *  function returns NULL if parsing was unsuccesfull
*/
char	*cd_parse(t_msdata *data, char **argv);

/**
 * @brief
 * This function retrieves the oldpwd and passes it as string back
 * @param t_msdata *data
 * @param char **argv
 * @param char *cdpath
 * @return
 * Function returns int to indicate succesful operation or not, 0 is
 * succesful operation 1 is unsuccesful operation
 * @exception
 *  function goed to error function if oldpwd
 *  cannot be retrieved due to malloc error
*/
int		*cd_parse_oldpwd(t_msdata *data, char **argv, char **cdpath);

/**
 * @brief
 * This function parses the argv to create the line to be printed
 * for echo, and if the -n flag was used
 * @param t_msdata *data
 * @param int *print_nl
 * @return
 * Function returns malloced char * of parsed line to be printed
 * @exception
 *  function returns NULL if parsing was unsuccesfull due to malloc errors
*/
char	*echo_getline(char **argv, int *print_nl, int arglen);

/**
 * @brief
 * This function will check if char *arg param is a -n flag
 * @param t_msdata *data
 * @param int *print_nl
 * @return
 * 0: arg is a -n flag;
 * 1: arg is not -n flag
*/
int		echo_check_nl(char *arg);

/**
 * @brief double check pso dir
 * @param char *dirname
 * @note if dir does not exists it free dirname
 */
void	cd_parse_pso_check(char **dirname);

/**
 * @brief checks if env can be added or not to envp in data struct
 * @param t_msdata *data
 * @param char *input
 * @param char *key
 * @param char *value
 */
void export_envp(t_msdata *data, char *input, char *key, char *value);


#endif	// BUILTINS_H
