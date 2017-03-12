#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

static void Usage(const char* msg)
{
	printf("Usage: %s [local_port] [local_ip]\n", msg);
}

int Startup(int port, char *ip)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0)
	{
		perror("socket");
		exit(1);
	}
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(local);

	if( bind(listen_sock, (struct sockaddr*)&local, len) < 0)
	{
		perror("bind");
		close(listen_sock);
		exit(2);
	}
	
	if(listen(listen_sock, 5) < 0)
	{
		perror("listen");
		close(listen_sock);
		exit(3);
	}
	return listen_sock;
}

void *ThreadRun(void *arg)
{
	int sock = *((int *)arg);
	char buf[1024];
	while(1)
	{
		ssize_t s = read(sock, buf, sizeof(buf)-1);
		if(s > 0)
		{
			buf[s] = 0;
			printf("client:%s\n", buf);
		}
		else if(s == 0)
		{
			printf("client is quit!\n");
			break;
		}
		else
		{
			perror("read");
		}
		write(sock, buf, strlen(buf));
	}
	close(sock);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	int listen_sock = Startup(atoi(argv[1]), argv[2]);
	
	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);	
		int connfd = accept(listen_sock, (struct sockaddr*)&peer, &len);
		if(connfd < 0)
		{
			perror("accept");
			continue;
		}
		//等待成功
		printf("accept access! port:%d ip:%s\n", \
				ntohs(peer.sin_port), \
				inet_ntoa(peer.sin_addr.s_addr));
		pthread_t tid;
		int ret = pthread_create(&tid, NULL, ThreadRun, &connfd);
		if(ret != 0)
		{
			perror("pthread_create");
			close(connfd);
		}
		else
		{
			pthread_detach(tid);
		}
	}
	close(listen_sock);
	
	return 0;
}
