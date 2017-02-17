#include "comm.h"


static int CommSemSet(int nsems,int flag)
{
	key_t key = ftok(PATHNAME, PROJID);
	if(key < 0)
	{
		perror("ftok error");
		return -1;
	}
	int semid = semget(key, nsems, flag);
	if(semid < 0)
	{
		perror("semget error");
		return -2;
	}
	return semid;
}

int CreateSemSet(int nsems)
{
	return CommSemSet(nsems, IPC_CREAT | IPC_EXCL | 0666);
}

int GetSemSet()
{
	return CommSemSet(0, IPC_CREAT);//获取的时候可以不关注nsems；参数二还可以设置成0；
}

int InitSemSet(int semid, int semnum)
{
	union semun un;
	un.val = 1; //初始化数值设为1
	int ret = semctl(semid, semnum, SETVAL, un);
	if(ret < 0)
	{
		perror("semctl error");
		return -1;
	}
	return 0;
}

static int SemOP(int semid, int semnum, int op)
{
	struct sembuf s;//即s[1],只传入一个信号量集
	s.sem_num = semnum;
	s.sem_op = op;
	s.sem_flg = 0; //不关注，先设置为0
	int ret = semop(semid, &s, 1);
	if(ret < 0)
	{
		perror("semop error");
		return -1;
	}
	return 0;
}

int P(int semid, int semnum)
{
	return SemOP(semid, semnum, -1);
}

int V(int semid, int semnum)
{
	return SemOP(semid, semnum, 1);
}

int DestorySemSet(int semid)
{
	if(semctl(semid, 0, IPC_RMID) < 0)
	{
		perror("semctl error");
		return -1;
	}
	return 0;
}

