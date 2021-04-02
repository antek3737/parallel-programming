#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define SERVER 1
#define MAX 400 

int queueID;
int serverPID;

struct message
{
  long int mtype; 
  long int mtype1; 
  char mtext[MAX];
};

void handler(int sig)
{
  printf("SIGNAL SIGINT\n");
  if (msgctl(queueID, IPC_RMID, 0) == -1)
  {
    printf("Error queue couldn't be deleted. \n");
  }
  return;
}

int main()
{
  struct message msg;

  signal(SIGINT, handler);

  serverPID = getpid();

  key_t key = ftok(".", 'F');

  if ((queueID = msgget(key, IPC_CREAT | 0777)) == -1)
  {
    perror("Error queue couldn't be created\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("ID queue=%d \n", queueID);
  }

  while (1)
  {

    if ((msgrcv(queueID, &msg, MAX + sizeof(msg.mtype1), SERVER, MSG_NOERROR)) == -1)
    {
      perror("Error server couldn't received message. \n");
      exit(EXIT_FAILURE);
    }

    printf("Server:receiving: \n %s \n mtype %ld \n mtype1 %ld \n", msg.mtext, msg.mtype, msg.mtype1);

    int msize = strlen(msg.mtext);
    for (int i = 0; i < msize; i++)
    {
      msg.mtext[i] = toupper(msg.mtext[i]);
    }

    msg.mtype = msg.mtype1;        
    msg.mtype1 = (long)serverPID; 
    printf("Server:sending: \n %s \n mtype %ld \n mtype1 %ld \n", msg.mtext, msg.mtype, msg.mtype1);

    if ((msgsnd(queueID, &msg, sizeof(msg.mtext) + sizeof(msg.mtype1) + 1, 0)) == -1)
    {
      perror("Error couldn't add message to queue.\n");
      exit(EXIT_FAILURE);
    }
  }

}
