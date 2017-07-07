#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

#define _SIZE_ 1024 //sizeof(fd_set)*8

int fd_arr[_SIZE_];  //保存文件被描述符的数组
int max_fd = 0;

static void Init_fd_arr()
{
	int i = 0;
	for( ; i < _SIZE_; ++i)
	{
		fd_arr[i] = -1;
	}
}

static int Add_fd_arr(int fd)
{
	int i = 0;
	for(; i < _SIZE_; ++i)
	{
		if(fd_arr[i] == -1)
		{
			fd_arr[i] = fd;
			return 0;
		}
	}
	return 1;
}

static int Remove_fd_arr(int fd)
{
	int i = 0;
	for ( ; i < _SIZE_; ++i)
	{
		if(fd_arr[i] == fd)
		{
			fd_arr[i] = -1;
			return 0;
		}
	}
	return 1;
}

//将文件描述符重新设置进fd_set中并获取文件描述符的最大值
static int Reload_fd_set(fd_set *set)
{
	assert(set);
	int i = 0;
	for( ; i < _SIZE_; ++i)
	{
		if(fd_arr[i] != -1)
		{
			FD_SET(fd_arr[i], set);
			if(fd_arr[i] > max_fd)
			{
				max_fd = fd_arr[i];
			}
		}
	}
	return max_fd;
}

int Startup(const char* ip, int port)
{
	assert(ip);
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock < 0)
	{
		perror("socket");
		exit(2);
	}

	//客户端连着我，服务器端主动断开连接。
	//四次挥手服务器处于timewait时间,服务器无法重启。这个连接没有被释放，ip地址和端口号仍然被占用。
	//解决：复用端口号。保证服务器关掉后可以立即重启。
	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		perror("bond");
		exit(3);
	}

	if(listen(listen_sock, 5) < 0)
	{
		perror("listen");
		exit(4);
	}
	return listen_sock;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage : %s [local_ip] [local_port]\n", argv[0]);
		return 1;
	}
	int listen_sock = Startup(argv[1], atoi(argv[2]));
	printf("listen socket success\n");

	fd_set rfds;
	Init_fd_arr();
	Add_fd_arr(listen_sock);

	while(1)
	{
		max_fd = Reload_fd_set(&rfds);
		struct timeval timeout = {5, 0};//等待5秒

		switch(select(max_fd+1, &rfds, NULL, NULL, &timeout))
		{
		case 0:
			printf("time out !!!\n");
			break;
		case -1:
			perror("select");
			break;
		default:
			{//至少有一个文件描述符就绪
				int index = 0;
				for( ; index < _SIZE_; ++index)
				{
					//处理listen_sock
					if(index == 0 && fd_arr[index] != -1 \
							&& FD_ISSET(listen_sock, &rfds))
					{
						//listen_sock已经就绪
						struct sockaddr_in peer;
						socklen_t size = sizeof(peer);
						int conn_sock = accept(listen_sock, \
								(struct sockaddr*)&peer, &size);
						if(conn_sock != -1)
						{
							printf("get a new connect\n");
							if(1 == Add_fd_arr(conn_sock))
							{
								printf("fd_arr is full, close conn_sock\n");
								close(conn_sock);
							}
						}
						else
						{
							perror("accept");
							continue;
						}
					}
					else if(fd_arr[index] != -1 && FD_ISSET(fd_arr[index], &rfds))
					{	
						//处理读事件
						//read有问题，这里先假定读的数据小于缓冲区大小。
						char buf[1024];
						ssize_t s = read(fd_arr[index], buf, sizeof(buf)-1);
						if(s > 0)
						{
							buf[s] = 0;
							printf("client: %s\n", buf);
						}
						else if(s == 0)
						{
							printf("client is quit!\n");
							FD_CLR(fd_arr[index], &rfds);
							Remove_fd_arr(fd_arr[index]);
							close(fd_arr[index]);
						}
						else
						{
							perror("read");
						}
					}//else if
					else
					{
						//处理其他
					}
				}
			}
			break;
		}
	}
	return 0;
} 
