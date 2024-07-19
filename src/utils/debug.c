/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   debug.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: spenning <spenning@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 15:02:08 by spenning      #+#    #+#                 */
/*   Updated: 2024/07/19 19:40:27 by crasche       ########   odam.nl         */
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
		vprintf(format, ptr);
		va_end(ptr);
	}
}
