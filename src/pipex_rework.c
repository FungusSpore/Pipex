/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/27 17:50:09 by jianwong         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	int	**pipefds;
	int	append_mode;
	int	pipe_count;
	int	arg_count;

	if (check_input(argc, argv, &append_mode))
		return (1);
	pipe_count = 0;
	arg_count = 1 + append_mode;
	pipefds = make_pipes(argc, append_mode);
	if (append_mode)
		heredoc_to_cmd(pipefds, argv, &arg_count, &pipe_count);
	else
		input_to_cmd(pipefds, argv, &arg_count, &pipe_count);
	if ((argc > 5 && !append_mode) || (argc > 6 && append_mode))
		loop_cmd(pipefds, argv, &arg_count, &pipe_count);
	if (append_mode)
		append_to_output(pipefds, argv, &arg_count, &pipe_count);
	else
		cmd_to_output(pipefds, argv, &arg_count, &pipe_count);
}
