#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>

#define fifo "./fifo"

FILE * createFile()
{
    char PIDtext[20];
    sprintf(PIDtext, "%s%d%s", "out_", getpid(), ".txt");

    FILE *f = fopen(PIDtext, "w");

    if (f == NULL)
    {
        perror("Error file couldn't be created \n");
        return NULL;
    }

    puts("Client has created file");
    return f;
}

void fillFileWithReceived(FILE * f,int fd)
{
    char c;
    int test;
   
    while(1)
    {    
        int test = read(fd, &c, sizeof(char));
        if(test < 0)
        {
            perror("Read error\n");
            exit(EXIT_FAILURE);
        }
        else if(test == 0)
        {
            printf("There is no more char to get\n");
            close(fd);
            break;
        }
        else
        {
            printf("RECEIVING:%c\n",c);
            fputc(c, f);   
        }
    }

    close(fd);
    
}

void closeFile(FILE * f)
{
    if(fclose(f) != 0)
    {
        printf("Error file couldn't be closed.\n");
    }

    printf("Client has closed file\n");

}


int main(int argc, char * argv[])
{
    unsigned long numberOfClients=strtoul(argv[1], NULL, 10);
    
    if(numberOfClients == 0)
    {
        if(errno == ERANGE)
        {
            printf("The maximum size has been exceeded\n");
            exit(EXIT_FAILURE);
        }
    }

    if(numberOfClients > INT_MAX)
    {
        printf("Error the number of clients is too much\n");
        exit(EXIT_FAILURE);
    }

    int status;
    int childPID;
    int res;

    printf("Number of clients:%ld\n", numberOfClients);

    char buff[256];
    FILE * fp_in;
    fp_in=popen("ps ux | wc -l ", "r");

    fgets(buff, 256, fp_in);
    
    int number_of_processes = atoi(buff);
    int maxOfProcesses = 200;
    
    pclose(fp_in);

    if(maxOfProcesses < number_of_processes)
    {
        printf("The limit of processes has been exceeded, I can not start program.");
        exit(EXIT_FAILURE);
    }

    if(mkfifo(fifo, 0666) == -1)
    {
        if(errno == EEXIST)
        {
            printf("Fifo already eexist\n");
        }
    }
    else
    {
        printf("Client has made fifo\n");
    }

    int fd=open(fifo, O_RDONLY);

    if(fd < 0)
    {
      exit(EXIT_FAILURE);
    }

    FILE * file;
    
    for(int i=0; i<numberOfClients; i++)
    {
        switch (fork())
        {
        case -1:
           perror("Fork error\n");
        break;

        case 0: 
            file=createFile();
            fillFileWithReceived(file,fd);
            closeFile(file);     
            exit(EXIT_SUCCESS);  
        break;
        }

    } 

    for(int j=0; j<numberOfClients; j++)
    {
	childPID = wait(&status);
        
        if (childPID  == -1)
        {
            perror("ERROR. Wait() error.");
            exit(1);
	}
                 
    }  

    unlink(fifo);
    
    printf("Receiving by clients has beed ended.\n");
    exit(1);

}
