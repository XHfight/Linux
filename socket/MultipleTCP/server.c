#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define _QUEUELEN_ 10 

int Init(int port, const char *ip)
{
	struct sockaddr_in ser_sock;
	//1.创建监听socket,分配一个文件描述符。
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
	{
		perror("socket");
		return -1;
	}

	//2.绑定listenfd的源地址
	ser_sock.sin_family = AF_INET;
	ser_sock.sin_port = htons(port);
	ser_sock.sin_addr.s_addr = inet_addr(ip);
	if(bind(listenfd, (struct sockaddr *)&ser_sock, sizeof(ser_sock)) < 0)
	{
		perror("bind");
		close(listenfd);
		return -2;
	}

	//3.监听,使listenfd成为一个监听描述符
	if(listen(listenfd, _QUEUELEN_) < 0)
	{
		perror("listen");
		close(listenfd);
		return -3;
	}
	return listenfd;
}


int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [local_PORT] [local_IP]\n", argv[0]);
		return 1;
	}

	int listenfd = Init(atoi(argv[1]), argv[2]);
	if(listenfd >= 0)
		printf("初始化成功，等待accept\n");
	
	while(1)
	{
		struct sockaddr_in cli_sock;
		int len = sizeof(cli_sock);
		//阻塞等待
		int clientfd = accept(listenfd, (struct sockaddr*)&cli_sock, &len);
		if(clientfd  < 0)
		{
			perror("accept");
			close(listenfd);
			continue;
		}
		else
		{
			printf("get connect, ip : %s, port: %d\n", \
					inet_ntoa(cli_sock.sin_addr.s_addr), ntohs(cli_sock.sin_port));
		}
	
		pid_t f = fork();

		if(f < 0) //fork失败
		{
			close(clientfd);
			perror("fork");
			exit(0);
		}
		else if(f == 0)//child
		{
			//清理子进程方法二，让孙子进程变为孤儿进程，由系统自动回收。
			if(fork() > 0)
			{
				exit(1);
			}

			char buf[1024];
			while(1)
			{
				ssize_t sz = read(clientfd, buf, sizeof(buf)-1);
				if(sz > 0)
				{
					buf[sz] = 0;
					printf("client: %s\n", buf);
				}
				else
				{
					printf("client is quit\n");
					break;
				}
			}
			close(clientfd);
		}
		else //father
		{
			//清理子进程方法一
			//while(waitpid(-1, NULL, WNOHANG));//非阻塞等待子进程
			
			close(clientfd);
		}

	}	
	close(listenfd);
	return 0;
}
