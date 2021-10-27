#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
	int   childStatus;
	printf("Parent process's pid = %d\n", getpid());
	pid_t firstChild = fork();
	if(firstChild == -1){
		perror("fork() failed!");
		exit(1);
	} else if(firstChild == 0){
		// The first child process execute this
		printf("First child's pid = %d\n", getpid());
		sleep(10);
	} else{
		// Parent process executes this
		// Fork another child
		pid_t secondChild = fork();
		if(secondChild == -1){
			perror("fork() failed!");
			exit(1);
		} else if(secondChild == 0){
			// The second child process execute this
			printf("Second child's pid = %d\n", getpid());
			sleep(10);
		} else{
			// Parent process executes this to
			// wait for the second child
			pid_t childPid = waitpid(secondChild, &childStatus, 0);
			printf("The parent is done waiting. The pid of child that terminated is %d\n", childPid);
		}
	}
	printf("The process with pid %d is returning from main\n", getpid());
	return 0;
}

