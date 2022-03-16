#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int	main(void)
{
	struct stat		*statbuf;
	
	if (access("shiot", F_OK) == 0)
		printf("the file exists.\n");
	printf("it doesn't\n");
	return (0);
}
