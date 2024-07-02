/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 12:51:12 by spenning          #+#    #+#             */
/*   Updated: 2024/07/02 19:16:11 by spenning         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*execute_path_check(char **path_spl, char *cmd)
{
	int		index;
	char	*temp;
	char	*temp_cmd;
	int		check;

	index = 0;
	while (path_spl[index] != NULL)
	{
		temp = ft_strjoin(path_spl[index], "/");
		if (!temp)
			return (NULL);
		temp_cmd = ft_strjoin(temp, cmd);
		free(temp);
		if (!temp_cmd)
			return (NULL);
		// check = check_file(temp_cmd);
		// if (check == 0)
		// {
		check = access(temp_cmd, X_OK);
		if (check == 0)
			return (temp_cmd);
		else
			free(temp_cmd);
		// }
		// else
			// free(temp_cmd);
		index++;
	}
	return (NULL);
}

char	*execute_path(char	*cmd, t_msdata *data)
{
	char *path;
	char **path_spl;
	char *ret;

	path = get_envp(data, "PATH");
	if (!path)
		return (NULL);
	path_spl = ft_split(path, ':');
	if (!path_spl)
	{
		if (path)
			free(path);
		return (NULL);
	}
	ret = execute_path_check(path_spl, cmd);
	free_char_array(path_spl);
	if (ret == NULL)
		return (NULL);
	return (ret);
}


void	print_cmd(t_cmd *cmd, char *path_cmd)
{
	int i;
	ft_printf("cmd %s\n", path_cmd);
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		ft_printf("ARGV[%d]:%s\n", i, cmd->argv[i]);
		i++;
	}
	ft_printf("INFILE:%d\n", cmd->infd);
	ft_printf("OUTFILE:%d\n\n", cmd->outfd);
}

// REFERENCE: https://reactive.so/post/42-a-comprehensive-guide-to-pipex
// REFERENCE: https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html
// REFERENCE: https://janelbrandon.medium.com/understanding-the-path-variable-6eae0936e976


void	execute(t_msdata *data)
{
	t_cmd *cmd;
	char* path_cmd;
	pid_t pid;
	int pipefd[2];
	int wstatus;
	int statuscode;

	cmd = data->cmd_head;
	ft_printf("\n------------execution----------------\n\n");
	while (cmd)
	{
		if (cmd->pipe != NULL)
		{
			if (pipe(pipefd) == -1)
				error("pipe\n");
		}
		pid = fork();
		if (pid == 0)
		{
			if (cmd->pipe != NULL)
				dup2(stdout, pipefd[write]);
			if (!data->cmd_head == cmd)
				dup2(stdin, pipefd[read]);
			path_cmd = execute_path(cmd->cmd, data);
			print_cmd(cmd, path_cmd);
			execve(path_cmd, cmd->argv, data->envp);
			ft_printf("fail execve\n");
			close(pipefd[read]);
			close(pipefd[write]);
			exit(0);
		}
		close(pipefd[read]);
		close(pipefd[write]);
		cmd = cmd->pipe;
		ft_printf("here\n");
	}
	while(wait(&wstatus) != -1 || errno != ECHILD);
	if (WIFEXITED(wstatus))
	{
		statuscode = WEXITSTATUS(wstatus);
	}

	//while loop to wait for child waitpid(?, ?,?) exit_code
	//WIFSIGNAL()
		//whatever it is returned += 128
	//WIFEXIT()
	return ;
}
