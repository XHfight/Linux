#include "comm.h"

int main()
{
	int shmid = GetShm();
//	printf("shmid:%d\n", shmid);
	char* mem = (char*)shmat(shmid, NULL, 0);
	
	if((void*)mem == (void*)-1)
	{
		perror("shmat");
		return -1;
	}

//	sleep(10);
	int i = 0;
	while(1)
	{
		mem[i++] = 'A';
		i %= SIZE;
		sleep(1);
	}

	shmdt(mem);
	return 0;
}
