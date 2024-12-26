/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/27 01:17:44 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int	input_to_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count) 
{
	int		fd;
	char	*line;

	fd = open(argv[(*arg_count)++], O_RDONLY);
	if (fd < 0)
	{
		perror("open");
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

char	*get_stdin(char *keyword)
{
	char	*buffer;
	char	*line;
	char	*temp;
	
	buffer = ft_calloc(1, 1);
	if (!buffer)
		return (NULL);
	ft_printf("heredoc");
	line = get_next_line(0);
	while (ft_strncmp(line, keyword, -1))
	{
		temp = ft_strjoin(buffer, line);
		free(buffer);
		free(line);
		buffer = temp;
		ft_printf("heredoc");
		line = get_next_line(0);
	}
	free(line);
	return (buffer);
}

int	loop_additional_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int	end;

	end = 0;
	while (pipefds[end++]);
	while (*pipe_count < end - 3)
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
		perror("open");
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

	// ft_printf("check_input\n");
	if(check_input(argc, argv, &append_mode))
		return (1);
	pipe_count = 0;
	arg_count = 1 + append_mode;
	// ft_printf("make_pipes\n");
	pipefds = make_pipes(argc, append_mode);
	// ft_printf("input\n");
	input_to_cmd(pipefds, argv, &arg_count, &pipe_count);
	// ft_printf("loop\n");
	if ((argc > 5 && !append_mode) || (argc > 6 && append_mode))
		loop_additional_cmd(pipefds, argv, &arg_count, &pipe_count);
	// ft_printf("output\n");
	cmd_to_output(pipefds, argv, &arg_count, &pipe_count);
}
