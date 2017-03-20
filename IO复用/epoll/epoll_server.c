#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h>

typedef struct epbuf
{
	int fd;
	char buf[10240];
}epbuf_t, *epbuf_p, **epbuf_pp;

int startup(const char *ip, int port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0)
	{
		perror("socket");
		exit(2);
	}

	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	if(bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(listen_sock, 5) < 0)
	{
		perror("listen");
		exit(4);
	}
	return listen_sock;
}

static epbuf_p alloc_epbuf(int fd)
{
	epbuf_p ptr = (epbuf_p)malloc(sizeof(epbuf_t));
	if(ptr == NULL)
	{
		perror("malloc");
		exit(6);
	}
	ptr->fd = fd;
	return ptr;
}

static void free_epbuf(epbuf_p ptr)
{
	if(ptr != NULL)
		free(ptr);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [local_ip] [local_port]\n", argv[0]);
		return 1;
	}

	int listen_sock = startup(argv[1], atoi(argv[2]));
	printf("listen is success !\n");

	int epfd = epoll_create(256);
	if(epfd < 0)
	{
		perror("epoll_create");
		return 5; 
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = alloc_epbuf(listen_sock);
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);

	while(1)
	{
		int num = 0; 
		struct epoll_event evs[32];//存储发生的事件
		int maxevents = 32;
		int timeout = 2000;
		switch(num = epoll_wait(epfd, evs, maxevents, timeout))
		{
		case 0:
			printf("time out!\n");
			break;
		case -1:
			perror("epoll_wait");
			break;
		default:
			{
				int i = 0;
				for(; i < num; ++i)
				{
					int fd = ((epbuf_p)(evs[i].data.ptr))->fd;
					if(fd == listen_sock && (evs[i].events & EPOLLIN))
					{
						struct sockaddr_in peer;
						int len = sizeof(peer);
						int conn_sock = accept(fd, (struct sockaddr *)&peer, &len);
						if(conn_sock < 0)
						{
							perror("accept");
							continue;
						}
						struct epoll_event _ev;
						ev.events = EPOLLIN;
						ev.data.ptr = alloc_epbuf(conn_sock);
						epoll_ctl(epfd, EPOLL_CTL_ADD, conn_sock, &ev);
						printf("get a new connect!\n");
					}
					else if(fd != listen_sock && (evs[i].events & EPOLLIN))
					{
						char *buf = ((epbuf_p)(evs[i].data.ptr))->buf;
						ssize_t s = read(fd, buf, sizeof(buf)-1);
						if(s > 0)
						{
							buf[s] = 0;
							printf("client: %s\n", buf);
							ev.events = EPOLLOUT;
							epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
						}
						else if(s == 0)
						{
							free_epbuf(evs[i].data.ptr);
							evs[i].data.ptr = NULL;
							epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
							close(fd);
							printf("client is quit !\n");
						}
						else
						{
							perror("read"); //下次继续读
						}
					}
					else if(fd != listen_sock && (evs[i].events & EPOLLOUT))
					{
						const char *msg = "HTTP/1.0 200 OK \r\n\r\n<html><h1>hello world! </h1></html>\n"; 
						printf("write\n");
						write(fd, msg, strlen(msg));
						free_epbuf(evs[i].data.ptr);
						evs[i].data.ptr = NULL;
						epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
						close(fd);
					}
					else
					{
						//other events
					}
				}
			}
			break;
		}
	}
	close(epfd);
	return 0;
}
