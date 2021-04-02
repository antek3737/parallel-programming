#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include"semaphores.h"
	                                       
int main()
{	

	FILE *file = fopen("result.txt", "w"); 
	
	if (file == NULL)
	{
		printf("File couldn't be created.");
		return 1;
	}

   	printf("File has been created.\n");	 
   	int semID = create_new_semaphore_set(1017, 2);
 	int buffid = shmget(3745, 1, IPC_CREAT|0666);
	
	if(buffid == -1)
	{
	 	printf("Shared memory couldn't be created.\n");
	}
 	else
 	{
 		printf("Shared memory has been created: %d \n",buffid);
 	}
  	
	void * buffptr = shmat(buffid,0,0);
 	
	if(buffptr == (void*)-1)
	{
  		printf("Shared memory couldn't be attached.\n");
   	}
    else
	{
		printf("Shared memory has been attached. : %p\n", buffptr);
	}
	
	semaphore_v(semID, 1);
	char c = 'a';
	while(1)
	{
		semaphore_p(semID, 0);
		if(*(char*)buffptr == EOF)
		{
			semaphore_v(semID, 1);
			break;
		}
		else
		{
			c = *(char*)buffptr;
			printf("Received c:%c\n", c);
			fprintf(file,"%c", c);
			semaphore_v(semID, 1);
		}
	}

	printf("Receiving from manufacturer has ended. \n");
	fclose(file);
	
	if( shmdt(buffptr) == -1)
	{
		printf("Semaphore set couldn't be detached");
	}
	
	return 0;
}
