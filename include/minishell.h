/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: crasche <crasche@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/18 16:32:33 by crasche       #+#    #+#                 */
/*   Updated: 2024/05/23 21:36:40 by crasche       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define	MINISHELL_H

# include <stdio.h>
# include <stdlib.h>



// PARSING (Chriss)
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd 
{
	char	*cmd;
	char	**argv;
	char	*in;
	char	*out;
	s_cmd	*next;
}	t_cmd;

typedef enum e_token_type
{
	WORD,
	PIPE,
	third
}	t_token_type;
// END PARSING





#endif	// MINISHELL_H
