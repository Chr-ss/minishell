/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:32:33 by crasche           #+#    #+#             */
/*   Updated: 2024/06/28 17:16:10 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

/**
 * @brief
 * This function parses the directory argument passed for cd
 * @param t_msdata *data
 * @return
 * Function returns malloced char * of parsed path to directory
 * @exception
 *  function returns NULL if parsing was unsuccesfull
*/
char	*cd_parse(t_msdata *data);

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
char	*echo_getline(char **argv, int *print_nl);

/**
 * @brief
 * This function will check if char *arg param is a -n flag
 * @param t_msdata *data
 * @param int *print_nl
 * @return 
 * 0: arg is a -n flag;
 * 1: arg is not -n flag
*/
int	echo_check_nl(char *arg);

#endif	// BUILTINS_H

