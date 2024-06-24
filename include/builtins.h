/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:32:33 by crasche           #+#    #+#             */
/*   Updated: 2024/06/24 19:21:45 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

//[Description]
//This function parses the directory argument passed for cd
//[Parameters]
// t_msdata *data
//[Return]
//Function returns malloced char * of parsed path to directory
//[Error]
// function returns NULL if parsing was unsuccesfull
char	*cd_parse(t_msdata *data);

#endif	// BUILTINS_H

