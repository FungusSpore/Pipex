/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:08:08 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/23 18:56:28 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <fcntl.h>
#include <unistd.h>

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

char	*get_stdin(char *keyword)
{
	char	*buffer;
	char	*line;
	char	*temp;
	
	buffer = ft_calloc(1, 1);
	if (!buffer)
		return (NULL);
	line = get_next_line(0);
	while (ft_strncmp(line, keyword, -1))
	{
		temp = ft_strjoin(buffer, line);
		free(buffer);
		free(line);
		buffer = temp;
		line = get_next_line(0);
	}
	free(line);
	return (buffer);
}

char	*command_processing(char **argv, int start, int end, char **buffer)
{
	int	pid;

	while (start < end)
	{
		pid = fork();
		if (pid == 0)
			execve(, char *const *argv, char *const *envp)

	}
}

int	main(int argc, char *argv[])
{
	char	*buffer;
	int		append_mode;

	if (check_input(argc, argv, &append_mode))
		return (1);
	if (!append_mode)
		buffer = get_file_content(argv[1]);
	else
		buffer = get_stdin(argv[2]);
	if (!buffer)
	{
		if (!append_mode)
			ft_putstr_fd("Failed to get content\n", 2);
		else
			ft_putstr_fd("Failed to read stdin\n", 2);
		return (1);
	}
	command_processing(argv, 2 + append_mode, argc, &buffer);
	if (!append_mode)
		overwrite_file(argv[argc - 1]);
	else
		append_file(argv[argc - 1]);
	return (0);
}
