/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/26 21:00:31 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void	free_all(char **var)
{
	int	i;

	i = 0;
	while (var[i]) free(var[i++]); free(var);
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

int	input_to_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count) {
	int		fd;
	char	*line;

	fd = open(argv[(*arg_count)++], O_RDONLY);
	if (fd < 0)
	{
		ft_printf("open failed\n");
		return (1);
	}
	create_child_process(pipefds, argv, pipe_count, arg_count);
	close(pipefds[*pipe_count][0]);
	line = get_next_line(fd);
	while (line)
	{
		ft_putstr_fd(line, pipefds[*pipe_count][1]);
		line = get_next_line(fd);
	}
	close(pipefds[*pipe_count][1]);
	close(fd);
	(*pipe_count)++;
	(*arg_count)++;
	return (0);
}

int	loop_additional_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int	end;

	end = 0;
	while (pipefds[end++]);
	while (*pipe_count < end - 1)
	{
		create_child_process(pipefds, argv, pipe_count, arg_count);
		close(pipefds[*pipe_count][0]);
		close(pipefds[*pipe_count][1]);
		(*pipe_count)++;
		(*arg_count)++;
	}
	return (0);
}

int	cmd_to_output(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int fd;
	char *line;

	create_child_process(pipefds, argv, pipe_count, arg_count);
	close(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
	(*pipe_count)++;
	(*arg_count)++;
	fd = open(argv[*arg_count], O_WRONLY);
	if (fd < 0)
	{
		ft_printf("failed to open output file\n");
		return (1);
	}
	line = get_next_line(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
	while (line)
	{
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(pipefds[*pipe_count][0]);
	}
	close(pipefds[*pipe_count][0]);
	return (0);
}

int	main(int argc, char **argv)
{
	int	**pipefds;
	int	append_mode;
	int	pipe_count;
	int	arg_count;

	ft_printf("check_input\n");
	if(check_input(argc, argv, &append_mode))
		return (1);
	pipe_count = 0;
	arg_count = 1 + append_mode;
	ft_printf("make_pipes\n");
	pipefds = make_pipes(argc, append_mode);
	ft_printf("input\n");
	input_to_cmd(pipefds, argv, &arg_count, &pipe_count);
	ft_printf("loop\n");
	if ((argc > 5 && !append_mode) || (argc > 6 && append_mode))
		loop_additional_cmd(pipefds, argv, &arg_count, &pipe_count);
	ft_printf("output\n");
	cmd_to_output(pipefds, argv, &arg_count, &pipe_count);
}
