/*************************************************************************
	> File Name: ProgressBar.c
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Fri 06 Jan 2017 11:23:23 PM CST
 ************************************************************************/

#include<stdio.h>

void progressBar()
{
	int rate = 0;
	char bar[102];
	char rotate[] = {'-', '\\', '|', '/'};
	bar[0] = '\0';

	while(rate <= 100)
	{
		printf("[%-100s][%d%][%c]\r", bar, rate, rotate[rate%4]);
		fflush(stdout);
		
		bar[rate] = '=';
		++rate;
		bar[rate] = '\0';
		usleep(100000);
	}
	printf("\n");
}
int main()
{
	progressBar();
	return 0;
}
