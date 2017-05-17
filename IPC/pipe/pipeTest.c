/*************************************************************************
	> File Name: pipeTest.c
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Tue 14 Feb 2017 10:22:04 PM CST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	//1.创建管道
	int fd[2] = {0, 0}; //文件描述符
	if(pipe(fd) != 0) //失败
	{
		perror("pipe failed");
		return 1;
	}

	//2.创建子进程
	pid_t id = fork();
	if(id == 0) //child
	{
		printf("child is runing!\n");
		sleep(2);
		//3.关闭对应的通信端    0：读  1：写
		//假如让子进程写
		close(fd[0]);

		const char* msg = "hello world!";
		int count = 5;
		while(count--)
		{
			write(fd[1], msg, strlen(msg));
			sleep(1);
		}
		close(fd[1]);
		exit(0);
	}
	else //father
	{
		int count = 5;
		char buf[1024];
		printf("father is runing!\n");
		sleep(2);
		close(fd[1]);
		while(count--)
		{
			ssize_t _s = read(fd[0], buf, sizeof(buf)-1);
			if(_s > 0)
			{
				buf[_s] = '\0';
				printf("%s\n", buf);
			}
			else
			{
				perror("read error");
				exit(1);
			}
		}
	}
	pid_t ret = waitpid(id, NULL, 0);
	if(ret == id)
	{
		printf("wait child success!\n");
	}
	else
	{
		printf("wait child failed!\n");
	}
	return 0;
}
