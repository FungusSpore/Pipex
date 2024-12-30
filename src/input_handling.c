/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:04:46 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/30 15:46:38 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	input_to_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	int		fd;
	char	*line;

	fd = open(argv[(*arg_count)++], O_RDONLY);
	if (fd < 0)
		perror("open");
	if (create_child_process(pipefds, argv, pipe_count, arg_count))
		return (1);
	close(pipefds[*pipe_count][0]);
	line = get_next_line(fd);
	while (line)
	{
		ft_putstr_fd(line, pipefds[*pipe_count][1]);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(pipefds[*pipe_count][1]);
	close(fd);
	(*pipe_count)++;
	(*arg_count)++;
	return (0);
}

static char	*get_stdin(char *keyword)
{
	char	*buffer;
	char	*line;
	char	*temp;

	buffer = ft_calloc(1, 1);
	if (!buffer)
		return (NULL);
	ft_printf("heredoc> ");
	line = get_next_line(0);
	while (ft_strncmp(line, keyword, ft_strlen(keyword)) \
		|| line[ft_strlen(keyword)] != '\n')
	{
		temp = ft_strjoin(buffer, line);
		free(buffer);
		free(line);
		buffer = temp;
		ft_printf("heredoc> ");
		line = get_next_line(0);
	}
	free(line);
	return (buffer);
}

int	heredoc_to_cmd(int **pipefds, char **argv, int *arg_count, int *pipe_count)
{
	char	*line;

	line = get_stdin(argv[(*arg_count)++]);
	if (create_child_process(pipefds, argv, pipe_count, arg_count))
		return (1);
	close(pipefds[*pipe_count][0]);
	ft_putstr_fd(line, pipefds[*pipe_count][1]);
	close(pipefds[*pipe_count][1]);
	free(line);
	(*pipe_count)++;
	(*arg_count)++;
	return (0);
}
