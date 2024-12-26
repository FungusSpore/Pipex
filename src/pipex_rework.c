/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/26 14:02:24 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <stdlib.h>
#include <unistd.h>

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

int	main(int argc, char **argv)
{
	int	**pipefd;
	int	append_mode;

	check_input(argc, argv, &append_mode);
	pipefd = make_pipes(argc, append_mode);
	input_to_cmd();
	loop_additional_cmd();
	cmd_to_output();
}
