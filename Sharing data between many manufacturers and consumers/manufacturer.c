#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>

#define fifo "./fifo"


FILE * createFile()
{
    char PIDtext[20];
    sprintf(PIDtext,"%s%d%s", "in_", getpid(), ".txt");

    FILE *f = fopen(PIDtext, "w");
    if (f == NULL)
    {
        perror("Error file couldn't be created \n");
        return NULL;
    }
    puts("Manufacturer has created file.");
    return f;
}

void fillFileWithSent(FILE * f, int limit)
{
    char c;

    int fd = open(fifo,O_WRONLY);

    for(int i=0; i<limit; i++)
    {   
        c = 33 + rand()%90;
   
        printf("Sending:%c\n", c);
        fputc(c, f);
       
       if(write(fd, &c, sizeof(char)) < 0)
       {
           printf("Write() ERROR\n");
           break;
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
    printf("Manufacturer has closed the file.\n");
}

int main(int argc ,char * argv[])
{
    srand(time(NULL));
    int status;
    int childPID;
    int res;
    unsigned long numberOfManufacturers = strtoul(argv[1],NULL,10);
    
   if(numberOfManufacturers == 0)
    {
        if(errno == ERANGE)
        {
            printf("The maximum size has been exceeded\n");
            exit(EXIT_FAILURE);
        }
    }
    
   if(numberOfManufacturers > INT_MAX)
    {
        printf("Error the number of manufacturers is too much \n");
        exit(EXIT_FAILURE);
    }

    unsigned  long limit = strtoul(argv[2],NULL,0);
    
    if(limit == 0)
    {
        if(errno == ERANGE)
        {
            printf("The maximum size has been exceeded\n");
            exit(EXIT_FAILURE);
        }
    
    printf("The form of inputed limit is inappropriate \n");
    exit(EXIT_FAILURE);    
    }

     if(limit > INT_MAX)
    {
        printf("The limite is too much \n");
        exit(EXIT_FAILURE);
    }

    printf("Number of manufacturers :%ld\nLIMIT:%ld\n", numberOfManufacturers, limit);

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
     
    if(mkfifo(fifo,0666) == -1){
        if(errno == EEXIST)
        {
            printf("FIFO ALREADY EEXIST\n");
        }
    }
    else
    {
        printf("Manufacturer has made fifo\n");
    }

    FILE * file;
    for(int i=0; i<numberOfManufacturers; i++)
    {
        switch (fork())
        {
        case -1:
        printf("FORK error\n");
        break;
        
        case 0: 
        file = createFile();
        fillFileWithSent(file,limit);
        closeFile(file);        
        exit(EXIT_SUCCESS);  
        break;
        }

    }

    for(int j=0 ; j<numberOfManufacturers; j++)
    {
        childPID = wait(&status);
     
        if (childPID  == -1)
        {
            perror("ERROR. Wait() error.");
            exit(1);
        }

    } 

    printf("Sending by manufacturers has beed ended. \n");

    exit(1);

}