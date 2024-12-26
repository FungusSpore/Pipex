#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	int fd = open("test.txt", O_RDONLY);
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	return EXIT_SUCCESS;
}
