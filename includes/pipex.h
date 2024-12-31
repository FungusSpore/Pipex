/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:08:19 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/31 14:56:58 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <string.h>
# include <sys/wait.h>
# include <stdio.h>

# define APD_MODE 0
# define PIPE_IDX 1
# define ARG_IDX 2
# define ERROR 3

int		check_input(int argc, char **argv, int *append_mode);

int		**make_pipes(int argc, int append_mode);
int		create_child_process(int **pipefds, char **argv, \
	int *pipe_count, int *arg_count);

int		heredoc_to_cmd(int **pipefds, char **argv, \
	int *arg_count, int *pipe_count);
int		input_to_cmd(int **pipefds, char **argv, \
	int *arg_count, int *pipe_count);

int		cmd_to_output(int **pipefds, char **argv, \
	int *arg_count, int *pipe_count);
int		append_to_output(int **pipefds, char **argv, \
	int *arg_count, int *pipe_count);

#endif // !PIPEX_H
