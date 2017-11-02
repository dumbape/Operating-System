#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
int main()
{
	char *myfifo="myfifo";
	
	char str[80];
	int fd;
	
	while(1)
	{
		fd=open(myfifo,O_RDONLY);
	
		if(fd>=0) { 
		printf("Waiting for input...");
		
		read(fd,str,80)!=0;
		if(strcmp(str,"exit")==0)
		{	
			printf("Exiting");
			close(fd);
			break;
		}
		printf("Got it:%s\n",str);
		close(fd);
	}
	}
	
	return 0;
}
