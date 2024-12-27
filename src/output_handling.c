/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:39:55 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/27 17:51:45 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	cmd_to_output(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int		fd;
	char	*line;

	create_child_process(pipefds, argv, pipe_count, arg_count);
	close(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
	(*pipe_count)++;
	(*arg_count)++;
	close(pipefds[*pipe_count][1]);
	fd = open(argv[*arg_count], O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		return (1);
	}
	line = get_next_line(pipefds[*pipe_count][0]);
	while (line)
	{
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(pipefds[*pipe_count][0]);
	}
	close(pipefds[*pipe_count][0]);
	return (0);
}

int	append_to_output(int **pipefds, char **argv, \
int *arg_count, int *pipe_count)
{
	int		fd;
	char	*line;

	create_child_process(pipefds, argv, pipe_count, arg_count);
	close(pipefds[*pipe_count][0]);
	close(pipefds[*pipe_count][1]);
	(*pipe_count)++;
	(*arg_count)++;
	close(pipefds[*pipe_count][1]);
	fd = open(argv[*arg_count], O_WRONLY | O_APPEND);
	if (fd < 0)
	{
		perror("open");
		return (1);
	}
	line = get_next_line(pipefds[*pipe_count][0]);
	while (line)
	{
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(pipefds[*pipe_count][0]);
	}
	close(pipefds[*pipe_count][0]);
	return (0);
}
