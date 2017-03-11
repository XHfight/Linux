#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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
	struct sockaddr_in cli_sock;

	int listenfd = Init(atoi(argv[1]), argv[2]);
	if(listenfd >= 0)
		printf("初始化成功，等待accept\n");
	
	int len = sizeof(cli_sock);
	while(1)
	{
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
	close(listenfd);
	//printf("this is server\n");
	return 0;
}
