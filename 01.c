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
sem_t mysem;
struct msg
{
	struct msg *next;
	int num;
};
struct msg *head;
pthread_t tid_p,tid_c;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *p)
{
	tid_p=pthread_self();
	struct msg *mp;
	int i;
	for(i=0;i<20;++i)
	{
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 1000+1;
		printf("p is %d\n",mp->num);
		pthread_mutex_lock(&lock);
		mp->next=head;
		head=mp;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&has_product);
		//sleep(rand()%5);
	}
}

void *consumer(void *p)
{
	tid_c=pthread_self();
	struct msg *mp;
	int i;
	for(i=0;i<20;++i)
	{
		pthread_mutex_lock(&lock);
		while(head==NULL)
		pthread_cond_wait(&has_product,&lock);
		mp=head;
		head=mp->next;
		pthread_mutex_unlock(&lock);
		printf("C is %d\n",mp->num);
		free(mp);
		//sleep(rand()%5);
	}
}

int main()
{
	int i,rfd,wfd,len=0,fd_in,fd1,fd2;
	char str[1024];
	
	pthread_t tid1,tid2;
	sem_init(&mysem,0,0);
	mkfifo("fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo("fifo2",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	rfd=open("fifo1",O_RDONLY);
	wfd=open("fifo2",O_WRONLY);
	if(rfd<0||wfd<0)return 0;
	printf("123");
	fflush(stdout);
	while(1)
	{
		
		memset(str,0,sizeof(str));
		if(read(rfd,str,sizeof(str)))
		{
			if(strcmp(str,"create producer\n")==0)
			{
				pthread_t tid1;
				pthread_create(&tid1,NULL,producer,NULL);
				char wstr[1024]={0};
				sprintf(wstr,"%lu",tid_p);
				write(wfd,wstr,strlen(wstr));
				pthread_join(tid1,NULL);
			}
			if(strcmp(str,"create consumer")==0)
			{
				pthread_t tid2;
				pthread_create(&tid2,NULL,consumer,NULL);
				pthread_join(tid2,NULL);	
			}
			if(strstr(str,"delete")!=NULL)
			{
				char tid[32];
				int k;
				strcpy(tid,str+6);
				pthread_t tid3;
				tid3 = atoi(tid);
				k = pthread_key_delete(tid3);
			}
		}
	}
	close(rfd);
	return 0;
}

