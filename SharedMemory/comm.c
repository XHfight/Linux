#include "comm.h"

static int CommShm(int flag)
{
	key_t key = ftok(PATHNAME, PROJID);
	int shmid = shmget(key, SIZE, flag);
	if(shmid < 0)
	{
		perror("shmget");
		return -1;
	}
	return shmid;
}

int CreateShm()
{
	return CommShm(IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm()
{
	return CommShm(0);
}

int DestoryShm(int shmid)
{
	if(shmctl(shmid, IPC_RMID, NULL) < 0)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}
