#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#define MIN 0
#define MAX 1
void CsigHandler(int sig)
{
	if(sig==SIGUSR1)
		printf("C has won");
	else if(sig==SIGUSR2)
		printf("C has lost");
}
void DsigHandler(int sig)
{
	if(sig==SIGUSR1)
		printf("D has won");
	else if(sig==SIGUSR2)
		printf("D has Lost");
}
int main()
{
	//char *pipe1="pipe1";
	//char *pipe2="pipe2";
	int c[2],d[2];
	time_t t;
	int ca,da;
	int chance=1;
	int i;
	char buf1[3],buf2[3];
	pipe(c);
	pipe(d);
	int num;
	
	
	if((ca=fork())==0)
	{	
		srand(time(NULL)<<2);
		close(d[0]);
		close(d[1]);
		close(c[0]);
		signal(SIGUSR1,NULL);
		signal(SIGUSR2,NULL);
		while(1)
		{
			
			int random;
			
			random=rand()%100;
			
			sprintf(buf1,"%d",random);
			write(c[1],buf1,3);
			sleep(1);
		}
		
	}
	
	if((da=fork())==0)
	{	
		srand(time(NULL)<<6);
		close(c[0]);
		close(c[1]);	
		close(d[0]);
		
		signal(SIGUSR1,NULL);
		signal(SIGUSR2,NULL);
		while(1)
		{	
			
			int random;
			random=rand()%100;
			
			sprintf(buf2,"%d",random);
			write(d[1],buf2,3);
			sleep(1);

		}
		
	}
	else
	{
		close(c[1]);
		close(d[1]);
		int round=1;
		int score_c=0,score_d=0;
		
		while(score_c!=10 && score_d!=10) { 
		printf("Round:%d with Choice of flag:",round);
		round++;
		chance=rand()%2;
		if(chance==MIN)
		{
			printf("MIN\n");
			read(c[0],buf1,3);
			read(d[0],buf2,3);
			int num1,num2;
			num1=atoi(buf1);
			num2=atoi(buf2);
			printf("Num recieved from C:%d\nNum recieved from D:%d\n",num1,num2);
			if(num1<num2) {
				score_c++;
				printf("C gets the point\n");
			}
			else {
				score_d++;
			printf("D gets the point\n");
			}
			
		}
		else if(chance==MAX)
		{
		printf("MAX\n");
			read(c[0],buf1,3);
			read(d[0],buf2,3);
			int num1,num2;
			num1=atoi(buf1);
			num2=atoi(buf2);
			printf("Num recieved from C:%d\nNum recieved from D:%d\n",num1,num2);
			if(num1>num2) {
				score_c++;
				printf("C gets the point\n");
			}
			else {
				score_d++;
			printf("D gets the point\n");
			}
				
		}
	printf("score of C:%d\nscore of D:%d\n",score_c,score_d);
	}
	if(score_c==10) {
		printf("C is winner\n");
		kill(ca, SIGUSR1);            
         kill(da, SIGUSR2);
	}
	else {
		printf("D is winner\n");
		kill(da, SIGUSR1);            
         kill(ca, SIGUSR2);
	}
	}

	exit(0);
	return 0;
}