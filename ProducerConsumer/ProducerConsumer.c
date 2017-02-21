#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

typedef int DataType;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct ListNode
{
	DataType _data;
	struct ListNode* _next; 
}Node, *pNode, **ppNode;

pNode CreateNode(DataType data)
{
	pNode newNode = (pNode)malloc(sizeof(Node));
	if(newNode == NULL)
	{
		perror("malloc");
		return NULL;
	}
	newNode->_data = data;
	newNode->_next = NULL;
	return newNode;
}

void DelNode(pNode node)
{
	if(node != NULL)
	{
		free(node);
		node = NULL;
	}
}

void InitList(ppNode h)
{
	assert(h);
	*h = CreateNode(0);
}

void PushHead(pNode list, DataType data)
{
	assert(list);
	pNode newNode = CreateNode(data);
	newNode->_next = list->_next;
	list->_next = newNode;
}

void PopHead(pNode list, DataType* data)
{
	assert(list);
	if(list->_next == NULL)
	{
		printf("list is empty!\n");
		return;
	}

	pNode del = list->_next;
	*data = del->_data;
	list->_next = del->_next;
	DelNode(del);
}

void DestroyList(ppNode list)
{
	assert(list);
	pNode cur = (*list)->_next;
	while(cur)
	{
		pNode del = cur;
		cur = cur->_next;
		DelNode(del);
	}

	DelNode(*list);
	*list = NULL;
}

int IsEmpty(pNode list)
{
	assert(list);
	return (list->_next == NULL);
}

void ShowList(pNode list)
{
	assert(list);
	pNode cur = list->_next;
	while(cur)
	{
		printf("%d ", cur->_data);
		cur = cur->_next;
	}
	printf("NULL\n");
}


void* ProducerThread(void* arg)
{
	pNode list = (pNode)arg;

	while(1)
	{
		int data = rand()%10000;
		pthread_mutex_lock(&lock);
		PushHead(list, data);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond);
		printf("producer : %d\n", data);
		usleep(100000);
	}
}

void* ConsumerThread(void* arg)
{
	pNode list = (pNode)arg;
	int data = 0;
	while(1)
	{
		pthread_mutex_lock(&lock);
		if(IsEmpty(list))
		{
			pthread_cond_wait(&cond, &lock);
		}

		PopHead(list, &data);
		pthread_mutex_unlock(&lock);
		printf("consumer : %d\n", data);
		usleep(20000);
	}
}

int main()
{
	pNode list = NULL;
	pthread_t proID = 0;
	pthread_t conID = 0;
	InitList(&list);

	int ret1 = pthread_create(&proID, NULL, ProducerThread, (void*)list);
	int ret2 = pthread_create(&conID, NULL, ConsumerThread, (void*)list);
	
	if(ret1 != 0 || ret2 != 0)
	{
		perror("pthread_create");
		return 1;
	}
	
	pthread_join(proID, NULL);
	pthread_join(conID, NULL);

	pthread_mutex_destroy(&lock);

	//测试单链表
//	int i = 0;
//	pNode list = NULL;
//	InitList(&list);
//
//	for( ; i<10; ++i)
//	{
//		PushHead(list, i);
//		ShowList(list);
//	}
//
//	printf("IsEmpty:%d\n", IsEmpty(list));
//	
//	for( ; i >= 0; --i)
//	{
//		int data = 0;
//		PopHead(list, &data);
//		ShowList(list);
//	}
//	printf("IsEmpty:%d\n", IsEmpty(list));
//	DestroyList(&list);
	return 0;
}
