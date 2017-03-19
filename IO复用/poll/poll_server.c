#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <assert.h>

#define _SIZE_ 10

int Startup(char* ip, int port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0)
	{
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
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

static void Init_fds(struct pollfd *fds, int len)
{
	assert(fds);
	int i = 0;
	for( ; i < len; ++i)
	{
		fds[i].fd = -1;
		fds[i].events = 0;
		fds[i].revents = 0;
	}
}

static int Add_fds(struct pollfd *fds, int len, int add_fd)
{
	assert(fds);
	int i = 0;
	for( ; i < len; ++i)
	{
		if(fds[i].fd == -1)
		{
			fds[i].fd = add_fd;
			return i;
		}
	}
	return -1;
}

int main(int arg, char *argv[])
{
	if(arg != 3)
	{
		printf("Usage: %s [local_ip] [local_port]\n",argv[0]);
		return 1;
	}

	int listen_sock = Startup(argv[1], atoi(argv[2]));
	printf("listen is success! \n");

	struct pollfd fds[_SIZE_]; //创建pollfd数组，记录文件描述符和关心的事件
	Init_fds(fds, _SIZE_);

	int index = Add_fds(fds, _SIZE_, listen_sock);
	if(index != -1)
	{
		fds[index].events = POLLIN;
	}
	int timeout = 2000;//单位：毫秒
	while(1)
	{
		//poll的返回值意义与select相同
		switch(poll(fds, _SIZE_, timeout))
		{
		case 0: //超时
			printf("time out !!!\n");
			break;
		case -1://错误
			perror("poll");
			break;
		default:
			{
				int i = 0;
				for( ; i < _SIZE_ ; ++i)
				{
					//处理listen_sock读事件
					if(fds[i].fd == listen_sock && (fds[i].revents & POLLIN))
					{
						struct sockaddr_in peer;
						socklen_t len = sizeof(peer);
						int conn_sock = accept(listen_sock, \
								(struct sockaddr*)&peer, &len);
						if(conn_sock != -1)
						{
							printf("get a new connect!\n");
							int in = Add_fds(fds, _SIZE_, conn_sock);
							if(in == -1) //add失败
							{
								close(conn_sock);
								printf("fds数组已满");
								break;
							}
							fds[in].events = POLLIN;
						}
					}
					else if(fds[i].fd != -1 && (fds[i].revents&POLLIN))//处理读事件
					{
						char buf[1024];
						ssize_t s = read(fds[i].fd, buf, sizeof(buf)-1);
						if(s > 0)
						{
							buf[s-1] = 0; //用-1去掉换行符
							printf("client:%s\n", buf);
						}
						else if(s == 0)
						{
							close(fds[i].fd);
							//在fds中删除对应的文件描述符及事件
							fds[i].fd = -1;
							fds[i].events = 0;
							fds[i].revents = 0;

							printf("client is quit!!\n");
						}
						else
						{
							perror("read");
						}
					}
					else
					{
						//处理其他事件
					}
				}
				break;
			}
		}
	}
	return 0;
}
