/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 00:14:26 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/30 15:27:40 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
		if (!pipefd[i] || pipe(pipefd[i]) == -1)
		{
			perror("pipe");
			free_all((void **)pipefd);
			return (NULL);
		}
	}
	return (pipefd);
}

static int	is_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static char	*ft_strtrim_modified(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*ptr;

	if (!s1 | !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	if (start >= end && end >= 0)
		return (NULL);
	if (is_set(s1[start], set))
		start++;
	if (is_set(s1[end], set))
		end--;
	if (start > end)
		return (ft_strdup(""));
	ptr = malloc(sizeof(char) * (end - start + 2));
	if (!ptr)
		return (NULL);
	ft_strlcpy(ptr, s1 + start, end - start + 2);
	return (ptr);
}

void	cmd_processing(char **cmd)
{
	int		i;
	char	*line;

	i = 0;
	while (cmd[i])
	{
		line = ft_strtrim_modified(cmd[i], "\'\"");
		if (line)
		{
			free(cmd[i]);
			cmd[i] = line;
		}
			i++;
	}
}

int	create_child_process(int **pipefds, char **argv, \
int *pipe_count, int *arg_count)
{
	char	*path;
	char	**cmd;
	int		pid;

	cmd = ft_split_ignore_qoutes(argv[*arg_count], ' ');
	if (!*cmd)
	{
		free(cmd);
		return (1);
	}
	cmd_processing(cmd);
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
		if (execve(path, cmd, NULL) == -1)
			return (1);
	}
	free_all((void **)cmd);
	free(path);
	return (0);
}
