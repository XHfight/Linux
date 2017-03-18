#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("login", O_CREAT | O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return 1;
	}
	
	close(1);

	int ret = dup2(fd, 1);
	if(ret == -1)
	{
		perror("dup2");
		return 2;
	}

	int count = 10;
	while(count--)
	{
		printf("hello world %d\n", count);
	}

	return 0;
}
