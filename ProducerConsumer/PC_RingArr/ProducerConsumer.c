//生产者消费者模型，有多个生产者和多个消费者
//实现：用固定大小的环形缓冲区， 这里用数组来实现,利用POSIX信号量和互斥锁实现

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUF_SIZE 50
#define PRODUCER_NUM 10
#define CONSUMER_NUM 10

int ringBuf[BUF_SIZE];

//定义互斥锁
pthread_mutex_t lock_pro = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_con = PTHREAD_MUTEX_INITIALIZER;

//定义信号量
sem_t blanks;
sem_t datas;

void* ProRun(void* arg)
{
	int i = 0;
	while(1)
	{
		pthread_mutex_lock(&lock_pro);
		sem_wait(&blanks);
		int data = rand()%10000;
		ringBuf[i] = data;
		printf("producer data:%d\n", data);
		i++;
		i %= BUF_SIZE;
		sem_post(&datas);
		pthread_mutex_unlock(&lock_pro);
	}
}

void* ConRun(void* arg)
{
	int i = 0;
	while(1)
	{
		pthread_mutex_lock(&lock_con);
		sem_wait(&datas);
		int data = ringBuf[i];
		printf("consumer data:%d\n", data);
		i++;
		i %= BUF_SIZE;
		sem_post(&blanks);
		pthread_mutex_unlock(&lock_con);
	}
}

int main()
{
	pthread_t proID,conID;

	//初始化信号量
	sem_init(&blanks, 0, BUF_SIZE);
	sem_init(&datas, 0, 0);

	int i;
	//创建线程
	for(i = 0; i < PRODUCER_NUM; ++i)
		pthread_create(&proID, NULL, ProRun, NULL);
	for(i = 0; i < CONSUMER_NUM; ++i)
		pthread_create(&conID, NULL, ConRun, NULL);

	//等待线程
	for(i = 0; i < PRODUCER_NUM; ++i)
		pthread_join(proID, NULL);
	for(i = 0; i < PRODUCER_NUM; ++i)
		pthread_join(conID, NULL);

	//销毁信号量
	sem_destroy(&blanks);
	sem_destroy(&datas);

	//解除互斥锁
	pthread_mutex_destroy(&lock_pro);
	pthread_mutex_destroy(&lock_con);
	return 0;
}
