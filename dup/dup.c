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
		return fd;
	}

	printf("fd is %d\n", fd);
	close(1);

	int newfd = dup(fd);
	printf("newfd is %d\n", newfd);
	if(newfd == -1)
	{
		perror("dup");
		return -1;
	}
	close(fd);
	
	int count = 10;
	while(count--)
	{
		printf("hello world! %d \n", count);
	}
	return 0;
}
