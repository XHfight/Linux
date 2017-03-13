#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

//socketpair的用法

int main()
{
	int fd[2];
	//参数一：UNIX域间套接字，参数二：面向字节流服务
	int ret = socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);
	if(ret < 0)
	{
		perror("socketpair");
		return 1;
	}

	pid_t id = fork();
	if(id == 0) //child 
	{
		close (fd[0]);
		char buf[1024];
		while(1)
		{
			strcpy(buf, "hello world!");
			write(fd[1], buf, strlen(buf));
			ssize_t s = read(fd[1], buf, sizeof(buf)-1);
			if(s > 0)
				buf[s] = 0;
			printf("%s\n", buf);
			sleep(1);
		}
	}
	else if(id > 0)//father
	{
		close (fd[1]);
		char buf[1024];
		while(1)
		{
			ssize_t s = read(fd[0], buf, sizeof(buf)-1);
			if(s > 0)
			{
				buf[s] = 0;
				printf("%s\n", buf);
			}
			buf[0] = 'H';
			write(fd[0], buf, strlen(buf));
		}
	}
	else
	{
		perror("fork");
		return id;
	}
	return 0;
}
