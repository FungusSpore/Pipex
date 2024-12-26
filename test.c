#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./libft/libft.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *cmd[] = { "ls", "-l", argv[1], (char *)0 }; // Use argv[1] as a directory
    char *env[] = { NULL };                          // Inherit environment
		char *path = ft_strjoin("/bin/", cmd[0]);
		int pid = fork();

		if (pid == 0)
		{
			if (execve(path, cmd, NULL) == -1) {
					perror("execve failed");
					exit(EXIT_FAILURE);
			}
		}
		printf("%s\n", get_next_line(0));
    return 0;
}

