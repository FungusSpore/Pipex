/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 00:14:26 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/31 18:12:38 by jianwong         ###   ########.fr       */
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
	if (ft_strchr(set, s1[start]))
		start++;
	if (ft_strchr(set, s1[end]))
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

int	call_process(int **pipefds, int *pipe_count, char *path, char **cmd)
{
	char	*mod_path;

	mod_path = NULL;
	dup2(pipefds[*pipe_count][0], STDIN_FILENO);
	dup2(pipefds[*pipe_count + 1][1], STDOUT_FILENO);
	close(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
	close(pipefds[*pipe_count + 1][0]);
	close(pipefds[*pipe_count + 1][1]);
	if (execve(path, cmd, NULL) == -1)
	{
		mod_path = ft_strjoin("/bin/", path);
		if (execve(mod_path, cmd, NULL) == -1)
			perror("execve");
		free(mod_path);
		free_all((void **)cmd);
		return (1);
	}
	return (0);
}

int	create_child_process(int **pipefds, char **argv, \
int *pipe_count, int *arg_count)
{
	char	**cmd;
	int		pid;

	cmd = ft_split_ignore_qoutes(argv[*arg_count], ' ');
	if (!*cmd)
	{
		free(cmd);
		return (1);
	}
	cmd_processing(cmd);
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		if (call_process(pipefds, pipe_count, cmd[0], cmd))
			exit(1);
	free_all((void **)cmd);
	return (0);
}
