#ifndef __COMM_H__
#define __COMM_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define PROJID 0x6666	
#define SIZE 4096*1 //申请共享内存的大小

int CreateShm();
int GetShm();
int DestoryShm(int shmid);

#endif
