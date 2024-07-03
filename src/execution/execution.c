/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenning <spenning@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 12:51:12 by spenning          #+#    #+#             */
/*   Updated: 2024/07/03 19:47:36 by spenning         ###   ########.fr       */
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

int add_command_to_argv(t_cmd** cmd_s, char **cmd)
{
	int	index;
	int	arglen;
	char	**new_argv;

	index = 0;
	arglen = double_array_len((*cmd_s)->argv);
	new_argv = ft_calloc(sizeof(char *) * (arglen + 2), 1);
	if (new_argv == NULL)
		return (1);
	new_argv[arglen + 1] = NULL;
	new_argv[0] = *cmd;
	while ((*cmd_s)->argv[index] != NULL)
	{
		copy_over_str(index + 1, index, new_argv, (*cmd_s)->argv);
		index++;
	}
	free_char_array((*cmd_s)->argv);
	(*cmd_s)->argv= new_argv;
	return (0);
}

void	execute(t_msdata *data)
{
	t_cmd *cmd;
	char* path_cmd;
	pid_t pid;
	int wstatus;
	int statuscode;

	cmd = data->cmd_head;
	ft_printf("\n------------execution----------------\n\n");
	while (cmd)
	{
		if (cmd->pipe != NULL)
		{
			if (pipe(cmd->pipe->pipefd) == -1)
				error("pipe error\n");
			ft_printf("pipefd[0] %d\n", cmd->pipe->pipefd[0]);
			ft_printf("pipefd[1] %d\n", cmd->pipe->pipefd[1]);
		}
		pid = fork();
		if (pid < 0)
			error("fork error\n");
		if (pid == 0)
		{
			if (cmd->pipe != NULL)
			{
				dup2(cmd->pipe->pipefd[WR], STDOUT_FILENO);
				close(cmd->pipe->pipefd[WR]);
				close(cmd->pipe->pipefd[RD]);
			}
			if (!(data->cmd_head == cmd))
			{
				dup2(cmd->pipefd[RD], STDIN_FILENO);
				close(cmd->pipefd[RD]);
			}
			path_cmd = execute_path(cmd->cmd, data);
			if(add_command_to_argv(&cmd, &path_cmd))
				error("copy over error\n");
			// print_cmd(cmd, path_cmd);
			execve(path_cmd, cmd->argv, data->envp);
			error("execute error\n");
		}
		ft_printf("parent\n");
		ft_printf("cmd %s\n", cmd->cmd);
		if (!(data->cmd_head == cmd))
		{
			if(close(cmd->pipefd[RD]) == -1)
				ft_printf("close fail\n");
		}
		if (cmd->pipe != NULL)
		{
			if(close(cmd->pipe->pipefd[WR]) == -1)
				ft_printf("close fail\n");
		}
		cmd = cmd->pipe;
	}
	while(waitpid(pid, &wstatus, 0) != -1 || errno != ECHILD);
	ft_printf("here2\n");
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
