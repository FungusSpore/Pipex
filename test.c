#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./libft/libft.h"

int main(int argc, char *argv[]) {
    // if (argc < 2) {
    //     fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
    //     exit(EXIT_FAILURE);
    // }

		int pipefd1[2];
		pipe(pipefd1);
		int pipefd2[2];
		pipe(pipefd2);
    char *cmd[] = { "cat",  argv[1], (char *)0 }; // Use argv[1] as a directory
    char *env[] = { NULL };                          // Inherit environment
		char *path = ft_strjoin("/bin/", cmd[0]);
		int pid = fork();

		if (pid == 0)
		{
			dup2(pipefd1[0], STDIN_FILENO);
			dup2(pipefd2[1], STDOUT_FILENO);
			close(pipefd1[1]);
			close(pipefd2[0]);
			if (execve(path, cmd, NULL) == -1) {
					perror("execve failed");
					exit(EXIT_FAILURE);
			}
		}
		close(pipefd1[0]);
		close(pipefd2[1]);
		write(pipefd1[1], "Hello\n", 7);
		write(pipefd1[1], "Hello\n", 7);
		write(pipefd1[1], "Hello\n", 7);
		close(pipefd1[1]);
		waitpid(pid, NULL, 0);
		char *line = get_next_line(pipefd2[0]);
		int i = 0;
		while (line)
		{
			printf("%d\n", ++i);
			printf("%s", line);
			line = get_next_line(pipefd2[0]);
		}
		close(pipefd2[0]);
    return 0;
}

