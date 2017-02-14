/*************************************************************************
	> File Name: myshall.c
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Mon 13 Feb 2017 07:08:02 PM CST
 ************************************************************************/
/*
 *模拟shell实现最简单的myshell
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	while(1)
	{
		printf("[xuhuan@localhost myshell]^ ");
		fflush(stdout);

		//读取命令行数据
		char line[1024];
		ssize_t s = read(0, line, 1024);
		if(s > 0) //读到内容
		{
			line[s] = '\0';
		}
		else
		{
			printf("read error!\n");
			continue;
		}
		
		//对读取的数据进行解析
		char* start = line;
		char* argv[10];
		argv[0] = start;
		int argvIndex = 1;
		while(*start)
		{
			if(isspace(*start) != 0)  //*start为空格
			{
				*start = '\0';
				start++;
				argv[argvIndex++] = start; 
			}
			else
			{
				start++;
			}
		}
		argv[argvIndex - 1] = NULL; //为什么下标要减一
	
		//创建子进程
		pid_t id = fork();
		if(id == 0) //child
		{
			execvp(argv[0], argv);
			perror("error");
			exit(1);
		}
		else //father
		{
			wait(NULL);
		}
	}
	return 0;
}
