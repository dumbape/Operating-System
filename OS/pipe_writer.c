#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
int main()
{
	char *myfifo="myfifo";
	printf("Creating named pipe:%s\n",myfifo);
	mkfifo(myfifo,0666);
	char str[80];
	int fd;
	
	while(1)
	{
		fd=open(myfifo,O_WRONLY);
		scanf("%s",str);
		if(strcmp(str,"exit")==0)
		{
			write(fd,str,80);
			unlink(myfifo);
			close(fd);
			break;
		}
		write(fd,str,80);
		close(fd);
	}
	unlink(myfifo);
	return 0;

}