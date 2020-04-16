#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFF_SIZE 1024

int main(){
    int status;
    char buff[BUFF_SIZE];
    int fd;
    fd=open("./input.txt",O_RDONLY);
    
    pid_t pid = fork();
 
    if(pid==0){  
        printf("children 1id :%d\n",getpid());
        read(fd,buff,10);
        puts(buff);
        close(fd);
        sleep(1);
        exit(7);
    }
    else{
        pid=fork();
        if(pid==0){

            printf("children 2id :%d\n",getpid());
            read(fd,buff,10);
            puts(buff);
            close(fd);
            sleep(1);
            exit(7);
        }
        else{
            wait(&status);
            if(WIFEXITED(status)){
                printf("children1 processes  exit-waits , parent id: %d\n",getpid());
                read(fd,buff,10);
                puts(buff);
                close(fd);
                sleep(1);
            }
            wait(&status);
            if(WIFEXITED(status)){
                printf("children2 processes  exit-waits\n");
                sleep(1);
            }
        }
    }
    

}