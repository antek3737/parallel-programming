#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include"semaphores.h"
	                                       
int main()
{	
    FILE *file = fopen("input.txt", "r"); 
	
	if (file == NULL) 
	{
		printf("File couldn't be opened");
		return 1;
	}

	printf("File has been opened!\n");
	int semID=create_new_semaphore_set(1017,2);
	int  buffid=shmget(3745, 1, IPC_CREAT|0666);
	   
	if(buffid == -1)
	{
		printf("Couldn't create shared memory.\n");
	}
	else
	{
		printf("Shared memory has been created. : %d\n",buffid);
	}

	void * buffptr=shmat(buffid,0,0);
	
	if(buffptr==(void*)-1)
	{
		printf("Couldn't be attached to shared memory.\n");
	}
	else
	{
		printf("Shared memory has been attached : %p\n",buffptr);
	}
	
	semaphore_p(semID,1);
	char c='a';
	
	while(c!=EOF)
	{	c=getc(file);
		printf("Char from file:%c \n",c);
		*(char*)buffptr=c;
	   	semaphore_v(semID,0);
		semaphore_p(semID,1);
   	}

	printf("Removing semaphore set\n");
	remove_semaphore_set(semID);
	printf("Sending from file has been ended \n");
	fclose(file);
	
	if( shmdt(buffptr)==-1)
	{
		printf("Shared memory couldn't be detached");
	}

	if(shmctl(buffid,IPC_RMID,0)==-1)
	{
		printf("Shared memory couldn't be removed");
    }
	return 0;
}
