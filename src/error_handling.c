/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:31:13 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/23 17:49:35 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static int	check_file(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open failed");
		return (1);
	}
	return (0);
}

int	check_input(int argc, char **argv, int *append_mode)
{
	int	i;
	int	fd1;
	int	fd2;

	i = 1;
	*append_mode = 0;
	if (argc < 4)
	{
		ft_printf("Invalid: ./pipex <file1> <command1> ... <comandn> <file2>\n");
		return (1);
	}
	if (!ft_strncmp(argv[i], "here_doc", -1))
	{
		*append_mode = 1;
		if (check_file(argv[argc - 1]))
			return (1);
		return (0);
	}
	if (check_file(argv[i]) || check_file(argv[argc - 1]))
		return (1);
	return (0);
}
