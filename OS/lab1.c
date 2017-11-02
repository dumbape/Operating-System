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
#include<limits.h>

int SHM(int size){
	return shmget(IPC_PRIVATE, size, 0777|IPC_CREAT);
}

void SHMdelete(int shmid){
	shmctl(shmid, IPC_RMID, 0);
}

int main(){
	int fid, i, j, status;
	int shmid = SHM(2 * sizeof(int));
	sem_t s1;
	sem_init(&s1, 0, 1);
	fid = fork();

	if(fid < 0){
		printf("COULD NOT BE FORKED\n");
	}
	else if(fid == 0){ // CHILD PROCESS
		int *buf;
		sem_wait(&s1);
		buf = (int *)shmat(shmid, 0, 0);
		for(i = 0; i < 2; i++){
			buf[i] = (i + 1) * (i + 2);
		}
		sem_post(&s1);
		shmdt(buf);
	}
	else{ // PARENT PROCESS
		wait(&status);
		int *buf;
		sem_wait(&s1);
		buf = (int *)shmat(shmid, 0, 0);
		for(i = 0; i < 2; i++){
			printf("%d\n", buf[i]);
		}
		shmdt(buf);
		sem_post(&s1);
		SHMdelete(shmid);
	}
}