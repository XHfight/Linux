#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s [local_port] [local_ip]\n", argv[0]);
		return 1;
	}

	int sock = socket(AF_INET ,SOCK_DGRAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in ser_sock;
	ser_sock.sin_family = AF_INET;
	ser_sock.sin_port = htons(atoi(argv[1]));
	ser_sock.sin_addr.s_addr = inet_addr(argv[2]);

	if(bind(sock, (struct sockaddr*)&ser_sock, sizeof(ser_sock)) < 0)
	{
		close(sock);
		perror("bind");
		return 3;
	}
	
	printf("等待接收消息\n");	
	
	while(1)
	{
		char* buf[1024];
		struct sockaddr_in peer;
		int len = sizeof(peer);
		//接收数据
		ssize_t ret = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&peer, &len);
		
		printf("ip:%s port:%d\n", inet_ntoa(peer.sin_addr.s_addr), ntohs(peer.sin_port));
		
		if(ret < 0)
		{
			perror("recvfrom");
		}
		else if(ret > 0)
		{
			buf[ret] = 0; 
			printf("client:%s\n", buf);
		}
	}

	close(sock);
	return 0;
}
