#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

static int create_new_semaphore_set( int key,int numberOfElements);
static void set_semaphore(int semid,int semNumber, int binaryValue);
static void semaphore_p(int semid,int semNumber);
static void semaphore_v(int semid, int semNumber);
static void remove_semaphore_set(int semid);

static int create_new_semaphore_set( int key,int numberOfElements)
{
  int semaphore=semget(key,numberOfElements,IPC_CREAT|IPC_EXCL|0666);
  
  if (errno==EEXIST) 
  {
    printf("Semaphore set exists, returning semID\n");
	  semaphore=semget(key,numberOfElements,IPC_CREAT|0666);    
 		return semaphore;
 	}
  else
  {
		printf("Semaphore has been created : %d \n",semaphore);
		printf("Setting semaphores on value 0\n");
		
    for(int i=0;i<numberOfElements;i++)
		{
		  set_semaphore(semaphore,i,0);
		}	
 			return semaphore;
  }

  printf("Couldn't create new semaphore set or get access to existed set.\n");
  exit(EXIT_FAILURE);
  
 }

static void set_semaphore(int semid,int semNumber, int binaryValue)
{
  int set_sem;
  set_sem=semctl(semid,semNumber,SETVAL,binaryValue);
  
  if (set_sem==-1)
  {
    printf("Couldn't set semaphore set.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semaphore set has been set.\n");
  }

}

static void semaphore_p(int semid,int semNumber)
{
  int temp;
  struct sembuf semBuff;
  semBuff.sem_num=semNumber;
  semBuff.sem_op=-1;
  semBuff.sem_flg=0;
  temp=semop(semid,&semBuff,1);
  if (temp==-1) 
  {
    if(errno==EINTR)
    {
	    semaphore_p(semid,semNumber);
	  }
	  else if(errno==EIDRM)
    {
	    printf("Semaphore set had been removed before\n");
	  }
	  else
    {
	    printf("Couldn't close semaphore\n");
	    exit(EXIT_FAILURE);
	  }
  }
  else
  {
    printf("Semaphore has been closed.\n");
  }

}


static void semaphore_v(int semid, int semNumber)
{
  int temp;
  struct sembuf semBuff;
  semBuff.sem_num=semNumber;
  semBuff.sem_op=1;
  semBuff.sem_flg=0;
  temp=semop(semid,&semBuff,1);
  
  if(temp==-1) 
  {
	  if(errno==EIDRM)
    {
      printf("Semaphore set had been removed before \n");
    }
    else
    {
      printf("Couldn't open semaphore \n");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    printf("Semaphore has been opened.\n");
  }

}


static void remove_semaphore_set(int semid)  
{
  int sem=semctl(semid,0,IPC_RMID);
  
  if(sem==-1)
  {
    printf("Couldn't remove semaphore.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Semaphore has been removed. : %d\n",sem);
  }

}

