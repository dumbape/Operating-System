#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define SHARED_MEM_SIZE sizeof(int)
int main()
{
	caddr_t B1,B2;
	B1=mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
                              MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   if (B1 == (caddr_t) -1)
     { perror ("error in mmap while allocating shared memory\n");
       exit (1);
     }
     B2=mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
                              MAP_ANONYMOUS | MAP_SHARED, -1, 0);
   if (B2 == (caddr_t) -1)
     { perror ("error in mmap while allocating shared memory\n");
       exit (1);
     }
    int *b1in,*b1out,*b2in,*b2out;
    b1in=(int *)B1;
    b1out=(int *)B1;
    b2in=(int *)B2;
    b2out=(int *)B2;
    sem_t b1,b2;
    sem_init(&b1,0,1);
    sem_init(&b2,0,1);
    int p1,p2,p3;
    if((p1=fork())==0)
    {
    	int i;
    	for(i=1;i<=1000;i++)
    	{
    		sem_wait(&b1);
    		*b1in=i;
    		sem_post(&b1);
    		
    		b1in++;
    	}
    	
    	sem_wait(&b1);
    	*b1in=0;
    	sem_post(&b1);
    }
    if((p2=fork())==0 && p1!=0)
    {	
    	int value,i;
    	do
    	{
    		sem_wait(&b1);
    		value=*b1out;
    		
    		if(value%2==0 && value!=0)
    		{
    			sem_wait(&b2);
    			*b2in=value;
    			sem_post(&b2);
    			b2in++;
    		}
    		sem_post(&b1);
    		b1out++;
    	}while(value!=0); 
    }
    if((p3=fork())==0 && p1!=0 && p2!=0)
    {	
    	int value,i;
    	do
    	{
    		sem_wait(&b2);
    		value=*b2out;
    		if(value%3==0&&value!=0)
    		{
    			printf("**%d**\n",value);
    		}
    		sem_post(&b2);
    		b2out++;
    	}while(value!=0);
    }
    sem_destroy(&b1);
    sem_destroy(&b2);
    int status;
    if(p1!=0)
    	waitpid(p1,&status,0);
    if(p2!=0)
    	waitpid(p2,&status,0);
    if(p3!=0)
    	waitpid(p3,&status,0);
    return 0;

}