/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 00:14:26 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/27 00:15:59 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_all(char **var)
{
	int	i;

	i = 0;
	while (var[i])
		free(var[i++]); 
	free(var);
}

int	**make_pipes(int argc, int append_mode)
{
	int	**pipefd;
	int	size;
	int	i;

	size = argc - 2 - append_mode;
	i = -1;
	pipefd = malloc(sizeof(int *) * (size + 1));
	if (!pipefd)
		return (NULL);
	pipefd[size] = NULL;
	while (++i < size)
	{
		pipefd[i] = malloc(sizeof(int) * 2);
		if (!pipefd[i])
		{
			free_all((char **)pipefd);
			return (NULL);
		}
		pipe(pipefd[i]);
	}
	return (pipefd);
}

int	create_child_process(int **pipefds, char **argv, int *pipe_count, int *arg_count)
{
	char	*path;
	char	**cmd;
	int		pid;

	cmd = ft_split(argv[*arg_count], ' ');
	path = ft_strjoin("/bin/", cmd[0]);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		dup2(pipefds[*pipe_count][0], STDIN_FILENO);
		dup2(pipefds[*pipe_count + 1][1], STDOUT_FILENO);
		close(pipefds[*pipe_count][0]);
		close(pipefds[*pipe_count][1]);
		close(pipefds[*pipe_count + 1][0]);
		close(pipefds[*pipe_count + 1][1]);
		execve(path, cmd, NULL);
	}
	free_all(cmd);
	free(path);
	return (0);
}
