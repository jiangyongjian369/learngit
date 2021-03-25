#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/select.h>
#include<errno.h>
#include<semaphore.h>
int main()
{
	char s[32];
	char r[32];
	int wfd,rfd;
	mkfifo("fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo("fifo2",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	wfd=open("fifo1",O_WRONLY);
	rfd=open("fifo2",O_RDONLY);
	while(1)
	{
		fgets(s,sizeof(s),stdin);
		write(wfd,s,strlen(s));
		read(rfd,r,sizeof(r));
		puts(r);
	}
	return 0;
}

