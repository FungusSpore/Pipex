/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/27 19:01:05 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static int	loop_cmd(int **pipefds, char **argv, \
										int *arg_count, int *pipe_count)
{
	int	end;

	end = 0;
	while (pipefds[end])
		end++;
	while (*pipe_count < end - 2)
	{
		if (create_child_process(pipefds, argv, pipe_count, arg_count))
			return (1);
		close(pipefds[*pipe_count][0]);
		close(pipefds[*pipe_count][1]);
		(*pipe_count)++;
		(*arg_count)++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	**pipefds;
	int	append_mode;
	int	pipe_count;
	int	arg_count;
	int	error;

	error = 0;
	if (check_input(argc, argv, &append_mode))
		return (1);
	pipe_count = 0;
	arg_count = 1 + append_mode;
	pipefds = make_pipes(argc, append_mode);
	if (append_mode)
		error += heredoc_to_cmd(pipefds, argv, &arg_count, &pipe_count);
	else if (!error)
		error += input_to_cmd(pipefds, argv, &arg_count, &pipe_count);
	if (((argc > 5 && !append_mode) || (argc > 6 && append_mode)) && !error)
		error += loop_cmd(pipefds, argv, &arg_count, &pipe_count);
	if (append_mode && !error)
		error += append_to_output(pipefds, argv, &arg_count, &pipe_count);
	else if (!error)
		error += cmd_to_output(pipefds, argv, &arg_count, &pipe_count);
	free_all((void **)pipefds);
	if (error)
		return (1);
	return (0);
}
