/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_rework.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/28 21:40:55 by jianwong         ###   ########.fr       */
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

/* vars[0] = append_mode
	vars[1] = pipe_count
	vars[2] = arg_count
	vars[3] = error
*/
int	main(int argc, char **argv)
{
	int	**pipefds;
	int	vars[4];

	vars[3] = 0;
	if (check_input(argc, argv, &vars[0]))
		return (1);
	vars[1] = 0;
	vars[2] = 1 + vars[0];
	pipefds = make_pipes(argc, vars[0]);
	if (vars[0])
		vars[3] += heredoc_to_cmd(pipefds, argv, &vars[2], &vars[1]);
	else if (!vars[3])
		vars[3] += input_to_cmd(pipefds, argv, &vars[2], &vars[1]);
	if (((argc > 5 && !vars[0]) || (argc > 6 && vars[0])) && !vars[3])
		vars[3] += loop_cmd(pipefds, argv, &vars[2], &vars[1]);
	if (vars[0] && !vars[3])
		vars[3] += append_to_output(pipefds, argv, &vars[2], &vars[1]);
	else if (!vars[3])
		vars[3] += cmd_to_output(pipefds, argv, &vars[2], &vars[1]);
	free_all((void **)pipefds);
	if (vars[3])
		return (1);
	return (0);
}
// int	main(int argc, char **argv)
// {
// 	int	**pipefds;
// 	int	append_mode;
// 	int	pipe_count;
// 	int	arg_count;
// 	int	error;
//
// 	error = 0;
// 	if (check_input(argc, argv, &append_mode))
// 		return (1);
// 	pipe_count = 0;
// 	arg_count = 1 + append_mode;
// 	pipefds = make_pipes(argc, append_mode);
// 	if (append_mode)
// 		error += heredoc_to_cmd(pipefds, argv, &arg_count, &pipe_count);
// 	else if (!error)
// 		error += input_to_cmd(pipefds, argv, &arg_count, &pipe_count);
// 	if (((argc > 5 && !append_mode) || (argc > 6 && append_mode)) && !error)
// 		error += loop_cmd(pipefds, argv, &arg_count, &pipe_count);
// 	if (append_mode && !error)
// 		error += append_to_output(pipefds, argv, &arg_count, &pipe_count);
// 	else if (!error)
// 		error += cmd_to_output(pipefds, argv, &arg_count, &pipe_count);
// 	free_all((void **)pipefds);
// 	if (error)
// 		return (1);
// 	return (0);
// }
