/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/26 18:23:08 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <atomic>
#include <bits/types/struct_itimerspec.h>
#include <cinttypes>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

char	*get_file_content(char *file)
{
	char	*buffer;
	char	*line;
	char	*temp;
	int		fd;

	buffer = ft_calloc(1, 1);
	if (!buffer)
		return (NULL);
	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		temp = ft_strjoin(buffer, line);
		free(buffer);
		free(line);
		buffer = temp;
		line = get_next_line(fd);
	}
	return (buffer);
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
		dup2(pipefds[*pipe_count + 1][1], STDIN_FILENO);
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

int	input_to_cmd(int *pipefd1, int *pipefd2, char *file, char *command)
{
	int		pid;
	int		fd;
	char	*line;
	char	**cmd;

	cmd = ft_split(command, ' ');
	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		dup2(pipefd1[0], STDIN_FILENO);
		dup2(pipefd2[1], STDOUT_FILENO);
		close(pipefd1[0]);
		close(pipefd1[1]);
		close(pipefd2[0]);
		close(pipefd2[1]);
		execve(const char *path, char *const *argv, char *const *envp);
	}
	close(pipefd1[1]);
	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		ft_putstr_fd(line, pipefd1[0]);
		line = get_next_line(fd);
	}
	close(pipefd1[0]);
	free(cmd);
	return (0);
}

int	loop_additional_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int	end;
	int pid;
	char	**cmd;

	end = 0;
	while (pipefds[end++]);
	while (*pipe_count < end - 1)
	{
		create_child_process(pipefds, argv, pipe_count, arg_count);
		close(pipefds[*pipe_count][0]);
		close(pipefds[*pipe_count][1]);
		free_all(cmd);
		(*pipe_count)++;
		(*arg_count)++;
	}
}

int	cmd_to_output(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int	pid;
	int fd;
	char **cmd;
	char *line;

	create_child_process(pipefds, argv, pipe_count, arg_count);
	close(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
	(*pipe_count)++;
	(*arg_count)++;
	fd = open(argv[*arg_count], O_WRONLY);
	line = get_next_line(pipefds[*pipe_count][0]);
	while (line)
	{
		ft_putstr_fd(line, fd);
		line = get_next_line(pipefds[*pipe_count][0]);
	}
	close(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
}

int	main(int argc, char **argv)
{
	int	**pipefds;
	int	append_mode;
	int	pipe_count;
	int	arg_count;

	pipe_count = 0;
	check_input(argc, argv, &append_mode);
	arg_count = 1 + append_mode;
	pipefds = make_pipes(argc, append_mode);
	input_to_cmd(pipefds[pipe_count], pipefds[pipe_count + 1], argv[arg_count], argv[arg_count + 1]);
	pipe_count++;
	arg_count += 2;
	if ((argc > 5 && !append_mode) || (argc > 6 && append_mode))
		loop_additional_cmd(pipefds, argv, &arg_count, &pipe_count);
	cmd_to_output(pipefds, argv, &arg_count, &pipe_count);
}
