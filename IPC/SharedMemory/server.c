#include "comm.h"

int main()
{
	int shmid = CreateShm();
	sleep(5);
//	printf("create shmid: %d\n", shmid);
	char* mem = (char*)shmat(shmid, NULL, 0);
	
	int count = 5;
	while(count--)
	{
		printf("%s\n", mem);
		sleep(1);
	}

	shmdt(mem);
	DestoryShm(shmid);
	return 0;
}
