#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#define SERVER 1
#define MAX 400
int queueID;
int clientPID;
pthread_t tid_send;
pthread_t tid_receive;

struct message
{
  long int mtype;
  long int mtype1;
  char mtext[MAX];
};

void sending(struct message *toSend){
    int ret;
    do {
        ret = msgsnd(queueID, toSend, MAX + sizeof(long int), IPC_NOWAIT);
        if(ret != -1)
        {
            return;
        }

        sleep(3);
    } while(ret == -1 && errno == EAGAIN);

    perror("Error while sending message.");
    exit(EXIT_FAILURE);
}

void *send(void *ptr)
{

  struct message toSend;
  while (2)
  {
    fgets(toSend.mtext, MAX, stdin);
    toSend.mtype = SERVER;
    toSend.mtype1 = (long)clientPID;
    printf("Client:sending: \n %s \n mtype %ld \n mtype1 %ld \n", toSend.mtext, toSend.mtype, toSend.mtype1);

    sending(&toSend);
  }
}

void *receive(void *ptr)
{
  sleep(10);
  struct message received;
  while (2)
  {
    if (msgrcv(queueID, &received, MAX + sizeof(received.mtype1), (long)clientPID, MSG_NOERROR) == -1)
    {
      if (errno == EINVAL)
      {
        perror("Server has been stopped while receiving data\n");
      }
      perror("Error while receiving message.\n");
      exit(EXIT_FAILURE);
    }
    printf("Client:receving: \n %s \n mtype %ld \n mtype1 %ld \n", received.mtext, received.mtype, received.mtype1);
  }
}

int main()
{
  clientPID = getpid();

  printf("clientPID: %d\n", clientPID);
  
  key_t key = ftok(".", 'F');

  if ((queueID = msgget(key, IPC_CREAT | 0777)) == -1)
  {
    perror("Errot cannot create message queue.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("ID queue=%d \n", queueID);
  }

  if (pthread_create(&tid_send, NULL, send, 0) < 0)
  {
    printf("pthread_create (send) error\n");
    exit(1);
  }

  if (pthread_create(&tid_receive, NULL, receive, 0) < 0)
  {
    printf("pthread_create (receive) error\n");
    exit(1);
  }

  if (pthread_join(tid_send, NULL) < 0)
  {
    printf("pthread_join (send) error\n");
    exit(1);
  }
  if (pthread_join(tid_receive, 0) < 0)
  {
    printf("pthread_join (receive) error\n");
    exit(1);
  }
}

