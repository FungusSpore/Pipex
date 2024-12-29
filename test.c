#include "libft/libft.h"
#include <stdio.h>

int main(void)
{
	char **nums = ft_split("", ' ');
	int i = 0;
	if (nums == NULL)
		printf("yes\n");
	if (*nums == NULL)
		printf("yes inner \n");
}
