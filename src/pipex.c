/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 13:26:24 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/31 16:31:17 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	check_input(int argc, char **argv, int *append_mode)
{
	int	i;

	i = 1;
	*append_mode = 0;
	if (argc < 5)
	{
		ft_printf("Invalid: ./pipex <file1> <command1> ... <comandn> <file2>\n");
		return (1);
	}
	if (!ft_strncmp(argv[i], "here_doc", -1))
		*append_mode = 1;
	return (0);
}

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

int	kill_child(void)
{
	pid_t	w;
	int		status;

	while (1)
	{
		w = wait(&status);
		if (w == -1)
			break ;
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status))
				return (WEXITSTATUS(status));
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	**pipefds;
	int	vars[4];

	vars[ERROR] = 0;
	if (check_input(argc, argv, &vars[APD_MODE]))
		return (1);
	vars[PIPE_IDX] = 0;
	vars[ARG_IDX] = 1 + vars[APD_MODE];
	pipefds = make_pipes(argc, vars[APD_MODE]);
	if (vars[APD_MODE])
		vars[ERROR] += heredoc_to_cmd(pipefds, argv, &vars[ARG_IDX], &vars[PIPE_IDX]);
	else if (!vars[ERROR])
		vars[ERROR] += input_to_cmd(pipefds, argv, &vars[ARG_IDX], &vars[PIPE_IDX]);
	if (((argc > 5 && !vars[APD_MODE]) || (argc > 6 && vars[APD_MODE])) && !vars[ERROR])
		vars[ERROR] += loop_cmd(pipefds, argv, &vars[ARG_IDX], &vars[PIPE_IDX]);
	if (vars[APD_MODE] && !vars[ERROR])
		vars[ERROR] += append_to_output(pipefds, argv, &vars[ARG_IDX], &vars[PIPE_IDX]);
	else if (!vars[ERROR])
		vars[ERROR] += cmd_to_output(pipefds, argv, &vars[ARG_IDX], &vars[PIPE_IDX]);
	free_all((void **)pipefds);
	vars[ERROR] += kill_child();
	if (vars[ERROR])
		return (1);
	return (0);
}
