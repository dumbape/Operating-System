#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
int from[1005],to[1005];
int size;
int m1,n1,m2,n2;
double a[1005][1005],b[1005][1005],c[1005][1005];
void *thread_fn(void *arg){
	long id =(long) arg;
	int i,j,k;
	for (i = from[id]; i <to[id] ; i++)
	{
		for (j = 0; j < n2; j++)
		{
			c[i][j] = 0;

			for (k = 0; k < n1; k++) {
				c[i][j] += a[i][k]*b[k][j];

			}

		}
	}
	return NULL;
}

int main(int argc,char *argv[]){
	int bite,tempsize,threadsize;
	char *file1=argv[2];
	char *file2=argv[4];
	char *buf=argv[6];
	threadsize=atoi(buf);
	FILE *fp;
	fp=fopen(file1,"r");
	fscanf(fp,"%d %d\n",&m1,&n1);
	//printf("%d %d\n",m1,n1);
	int i,j;
	tempsize=m1;
	char buffer[30];
	double temp;
	for(i=0;i<m1;i++)
		for(j=0;j<n1;j++)
		{
			fgets(buffer,sizeof(buffer),fp);
			if(buffer[0]!='#') {
				temp=atof(buffer);
	//			printf("%lf\n",temp);
				a[i][j]=temp;
			}
			else 
				j--;
		}
	fclose(fp);
	fp=fopen(file2,"r");
	fscanf(fp,"%d %d\n",&m2,&n2);
	//printf("%d %d\n",m2,n2);
	for(i=0;i<m2;i++)
		for(j=0;j<n2;j++)
		{
			fgets(buffer,sizeof(buffer),fp);

			if(buffer[0]!='#') {
				temp=atof(buffer);
	//			printf("%lf\n",temp);
				b[i][j]=temp;
			}
			else 
				j--;
		}
	fclose(fp);
	if(n1!=m2)
		return 0;
	pthread_t t1[threadsize];
	int indexer=0,round=0;

	bite=tempsize/threadsize;
	if(tempsize%threadsize>1)
		bite++;
	size=0;
	if(threadsize==2)
	{	
		if(tempsize%2!=0) { 
			from[0]=0;
			to[0]=tempsize/2;
			from[1]=(tempsize/2)+1;
			to[1]=tempsize-1;
		}
		else 
		{
			from[0]=0;
			to[0]=(tempsize/2);
			from[1]=(tempsize/2);
			to[1]=tempsize;
		}

		size=2;
	}
	else { 
		while(tempsize>0)
		{	round++;
			if(tempsize>=bite&& size <threadsize)
			{
				from[size]=indexer;
				indexer=bite*round;
				to[size]=indexer;
				size++;
			}
			else if(size< threadsize)
			{
				from[size]=indexer;
				indexer+=tempsize;
				to[size]=indexer;
				size++;
			}
			/*else if(size >= threadsize && tempsize <bite)
			  {
			  to[size-1]=m1;
			//	printf("%d\n",to[size-1]);
			tempsize=0;
			break;

			}*/
			tempsize-=bite;
		}
	}
	to[size-1]=m1;
	printf("%d-\n",m1);
	for(i=0;i<size;i++)
		printf("-%d 	%d-\n",from[i],to[i]);
	for(i=0;i<size;i++)
	{
		pthread_create(&t1[i],NULL,thread_fn,(void *)i);
	}
	for(i=0;i<size;i++)
	{
		pthread_join(t1[i],NULL);
	}
	printf("Exiting main\n");


/*	for(i=0;i<m1;i++)
	{
		for(j=0;j<n2;j++)
			printf("%lf ",c[i][j]);
		printf("\n");
	}
*/
	return 0;
}
