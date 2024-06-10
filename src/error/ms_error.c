/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_error.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/02 13:52:00 by crasche       #+#    #+#                 */
/*   Updated: 2024/06/04 18:17:13 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_error(char *msg)
{
	perror(msg);
	exit(1);
}