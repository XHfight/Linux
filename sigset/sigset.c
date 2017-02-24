#include <stdio.h>
#include <signal.h>

//信号集基本操作函数的使用

void PrintPending(const sigset_t* ppending)
{
	int i = 1 ;
	for( ; i < 32; ++i)
	{
		if(sigismember(ppending, i) == 1)
		{
			printf("1");
		}
		else if(sigismember(ppending, i) == 0)
		{
			printf("0");
		}
	}
	printf("\n");
}

void CustomRun()  //自定义的信号处理函数
{
	printf("signal is strl-c! \n");
}

int main()
{
	int count = 0;
	sigset_t block, oblock, pending;
	sigemptyset(&block);
	sigaddset(&block, SIGINT);
	
	signal(SIGINT, CustomRun);
	//添加阻塞
	sigprocmask(SIG_BLOCK, &block, &oblock); // 获取/ 修改当前调用者的信号屏蔽字
	while(1)
	{
		sigpending(&pending); //获取pending表
		PrintPending(&pending);
		sleep(1);
		++count;

		if(count == 10)
		{
			//取消阻塞
			//sigprocmask(SIG_UNBLOCK, &block, NULL);

			//恢复之前的信号屏蔽字/阻塞信号集
			sigprocmask(SIG_SETMASK, &oblock, NULL);
		}
	}
	return 0;
}
