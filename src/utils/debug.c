/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   debug.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 15:02:08 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/09 17:58:19 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdarg.h>

void	debugger(char *format, ...)
{
	va_list	ptr;

	if (DEBUG == 1)
	{
		va_start(ptr, format);
		ft_printf(format, ptr);
		va_end(ptr);
	}
}
