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
pthread_mutex_t pro_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t con_lock = PTHREAD_MUTEX_INITIALIZER;

//定义信号量
sem_t blanks;
sem_t datas;

void* ProRun(void* arg)
{
	int i = 0;
	while(1)
	{
		sem_wait(&blanks);
		pthread_mutex_lock(&pro_lock);
		int data = rand()%10000;
		ringBuf[i] = data;
		printf("producer data:%d\n", data);
		i++;
		i %= BUF_SIZE;
		pthread_mutex_unlock(&pro_lock);
		sem_post(&datas);
	}
}

void* ConRun(void* arg)
{
	int i = 0;
	while(1)
	{
		sem_wait(&datas);
		pthread_mutex_lock(&con_lock);
		int data = ringBuf[i];
		printf("consumer data:%d\n", data);
		i++;
		i %= BUF_SIZE;
		pthread_mutex_unlock(&con_lock);
		sem_post(&blanks);
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
	pthread_mutex_destroy(&pro_lock);
	pthread_mutex_destroy(&con_lock);
	return 0;
}
