/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_readline.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/26 17:45:15 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 17:45:31 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ms_readline(void)
{
	char *buffer;

	while (1)
	{
		buffer = readline("minishell:~$");
		if (!buffer)
			ms_error("readline malloc error.");
		printf("%s\n", buffer);
	}
}