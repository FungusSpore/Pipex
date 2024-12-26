/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jianwong <jianwong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:08:19 by jianwong          #+#    #+#             */
/*   Updated: 2024/12/27 00:16:26 by jianwong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <string.h>
# include <stdio.h>

int	check_input(int argc, char **argv, int *append_mode);

void	free_all(char **var);
int		**make_pipes(int argc, int append_mode);
int		create_child_process(int **pipefds, char **argv, int *pipe_count, int *arg_count);

#endif // !PIPEX_H
