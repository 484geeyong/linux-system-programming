#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>

static void sig_catch(int sig_no);

int main()
{
	int pid;
	int status;
	signal(SIGUSR1, sig_catch);

	if((pid=fork()) == 0){
		pause();
		printf("Child wake up\n");
		exit(0);
	}else{
		//sleep(1);
		kill(pid, SIGUSR1);
		wait(&status);
	}
}

static void sig_catch(int sig_no)
{
	switch(sig_no){
		case SIGUSR1:
			printf("PID %d caught signal SIGUSR1.\n", getpid());
			break;
		default:
			fprintf(stderr, "something wrong\n");
			exit(1);
	}
}
