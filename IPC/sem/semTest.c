#include "comm.h"


int main()
{
	int semid = CreateSemSet(1);
	sleep(5);
	if(InitSemSet(semid, 0) < 0)
	{
		perror("InitSemSet error");
		return -1;
	}

	pid_t id = fork();
	if(id == 0)//clild
	{
		int semid = GetSemSet();
		while(1)
		{
			P(semid, 0);
			printf("A");
			fflush(stdout);
			usleep(10020);
			printf("A");
			fflush(stdout);
			usleep(1001);
			V(semid, 0);
		}
	}
	else
	{
		while(1)
		{
			P(semid, 0);
			printf("B");
			fflush(stdout);
			usleep(2002);
			printf("B");
			fflush(stdout);
			usleep(20000);
			V(semid, 0);
		}

		wait(NULL);
	}

	DestorySemSet(semid);
	return 0;
}
