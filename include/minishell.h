/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/26 17:13:42 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define	MINISHELL_H

# include <stdio.h>
# include <stdlib.h>

# include "../libft/include/libft.h"
# include "lexer.h"

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	char			*in;
	char			*out;
	struct	s_cmd	*pipe;
}	t_cmd;

typedef	struct s_msdata
{
	t_cmd	*cmd;
}	t_msdata;

// FUNCTIONS:

void	ms_parsing(t_msdata *data)
void	ms_init_cmdlist(t_cmd *cmd)
char *ms_readline(void)



#endif	// MINISHELL_H
