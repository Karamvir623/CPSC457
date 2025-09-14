#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int arg_c, char **pp_arg_v)
{
	pid_t pid, pid_child_search;
	int toParent;
	int fromChild;
	int i = 0;

	while(i != 30){
		pid = fork();
		if (pid < 0) {
			fprintf(stderr, "Fork Failed");
			exit(-1);
		}
		else if (pid == 0){
			printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
			// sleep(1); // allow parent to wait
			// printf("Enter an integer between 0 and 255: ");
			// scanf("%d", &toParent);
			if( i == 10) exit(1);
			exit(0);

		}
		
		i++;
		
	}
	sleep(1);
	while (pid > 0) {
		pid_child_search = wait(&fromChild);
		if(WEXITSTATUS(fromChild) == 1) {
			printf("Parent: The treasure was found by child with PID %d\n", pid_child_search);
			break;
		}
		
	
	}
	
	
	
	
	printf("The parent is done\n");
	exit(0);
}
