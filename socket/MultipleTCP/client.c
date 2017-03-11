#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [server_PORT] [server_IP]\n", argv[0]);
		return 1;
	}

	int clientfd = socket(AF_INET, SOCK_STREAM, 0);

	if(clientfd < 0)
	{
		perror("socket");
		return 2;
	}
	
	struct sockaddr_in ser_sock;
	ser_sock.sin_family = AF_INET; 
	ser_sock.sin_port = htons(atoi(argv[1]));
	ser_sock.sin_addr.s_addr = inet_addr(argv[2]);

	if(connect(clientfd, (struct sockaddr *)&ser_sock, sizeof(ser_sock)) < 0)
	{
		perror("connect");
		return 3;
	}
	
	printf("connect success\n");
	
	while(1)
	{
		char buf[1024];

		printf("client: ");
		fflush(stdout);
		ssize_t sz = read(0, buf, sizeof(buf)-1);
		buf[sz-1] = 0;
		write(clientfd, buf, sz);
	}

	close(clientfd);
	//printf("this is client\n");
	return 0;
}
