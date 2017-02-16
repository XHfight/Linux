#ifndef __COMM_H__
#define __COMM_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJID 0X6666

union semun 
{
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
};

int CreateSemSet(int nsems);
int GetSemSet(int nsems);
int P(int semid, int semnum);
int V(int semid, int semnum);
int DestorySemSet(int semid);

#endif
