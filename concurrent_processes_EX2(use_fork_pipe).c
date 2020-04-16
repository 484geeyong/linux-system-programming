//포크와 ipc 인풋파일 아웃파일 만들고 두개의 파이프 생성하고 칠드런 두개 페런트프로세스 한개 패런트는 인풋파일로부터 데이터 읽어서 10개읽음 
//그 캐릭터를 그대로 파이프를 통해서 차일드로 보낸다 차일드1에서 읽은 데이터를 소문자일경우 대문자로 변경시켜서 2번째 파이프를 통해서 차일드2로 보낸다
//마지막 차일드는 아웃풋 파일로 롸이트

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFF_SIZE 1024

int main(){
    int fd[2],fd2[2];
    int input, output;
    int n; char buff[10];

    pipe(fd);
    pipe(fd2); 

    if(fork()>0){//부모프로세스 txt를 읽어와서 파이프1을 통해 자식1으로 보낸다.
        input=open("./input.txt",0);
        close(fd[0]);
        while ((n=read(input,buff,10))!=0)
        {
            write(fd[1],buff,n);
        }
        close(input);
        close(fd[1]);
        wait(0);
        
    }
    else if(fork()==0){//자식1프로세스에서는 파이프1을통해 읽어온 데이터를 대문자로 변경하여 파이프2를 통해 자식2로 보낸다.
        close(fd[1]);
        close(fd2[0]);
        while ((n=read(fd[0],buff,10))!=0)      
         {
            for(int i=0; i<10; i++){ //대소문자 바꾸기
                if((buff[i]>='a')&&(buff[i]<='z')){
                    buff[i]-=('a'-'A');
                }
            }
            write(fd2[1],buff,n); //파이프2로 보내기
        }
        close(fd[0]);
        close(fd2[1]);
        exit(0);
     
    }
    else{
        if(fork()==0){//자식2프로세스에서는 자식1에서 파이프2를통해온 데이터를 읽어 txt파일으로 만들어준다.
        close(fd2[1]);
        output=creat("./c1-p-c2.txt",0666);
        while ((n=read(fd2[0],buff,10))!=0)      
        {
            write(output,buff,n);
        }
        close(output);
        close(fd2[0]);
        exit(0);
        }
    
    }
    
}