#include <stdio.h>
#include <signal.h>

//仿照sleep写一个mysleep函数

void CustomRun()
{
	//nothing	
}

int MySleep(int seconds)
{
	struct sigaction act,oact;
	sigset_t block, oblock, susmask;
	
	//设定SIGALRM信号阻塞
	sigemptyset(&block);
	sigaddset(&block, SIGALRM);
	sigprocmask(SIG_BLOCK, &block, &oblock);

	//设置SIGALRM的处理方式
	act.sa_handler = CustomRun;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, &oact); //修改alarm的默认处理方式 
	
	//核心逻辑
	alarm(seconds); //设定闹钟
	susmask = oblock;
	sigdelset(&susmask, SIGALRM);

	sigsuspend(&susmask);
	//pause(); //使进程挂起直到有信号递达 
	//若使用pause，当在alarm后进程切出，且在设定时间到了之后仍没有切回时，当进程重新切回后pause就再也不能捕捉到alarm信号了，所以进程一直挂起。
	unsigned int ret = alarm(0); //取消闹钟

	sigaction(SIGALRM, &oact, NULL); //将alarm的默认处理方式改回
	sigprocmask(SIG_SETMASK, &oblock, NULL);

	return ret;
}
	
int main()
{
	int sec = 5;
	MySleep(sec);
	printf("sleep %d second done!\n", sec);
	return 0;
}
