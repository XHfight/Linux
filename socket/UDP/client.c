#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argv, char* argc[])
{
	if(argv != 3)
	{
		printf("Usage: %s [server_port] [server_ip]", argc[0]);
		return 1;
	}

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(atoi(argc[1]));
	peer.sin_addr.s_addr = inet_addr(argc[2]);
	
	while(1)
	{
		printf("client:");
		fflush(stdout);

		char buf[1024];
		ssize_t s = read(0, buf, sizeof(buf)-1);
		if(s < 0)
		{
			perror("read");
			break;
		}
		buf[s-1] = 0;

		int ret = sendto(sock, buf, s, 0, (struct sockaddr*)&peer, sizeof(peer));
		if(ret < 0)
		{
			perror("sendto");
		}
	}

	close(sock);
	return 0;
}
