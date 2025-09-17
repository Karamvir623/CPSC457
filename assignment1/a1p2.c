#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>

#define MAX_PRIMES_PER_CHILD 1000000 //ask about what range should be appropriate

// function to test if input is a prime number
int is_prime(int num) {
	if (num < 2) return 0;
	for (int i = 2; i <= sqrt(num); i++) {
		if (num % i == 0) return 0;
	}
	return 1;
}

int main() {
	//memory layout suggested by assignment sheet
	int shm_size =  (n * MAX_PRIMES_PER_CHILD) * sizeof(int) // shared memory segment size
	int shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666); // shared memory creation
	int *shm_ptr = (int *) shmat(shmid, NULL, 0) // attaches shared memory block
	
	//example input
	int upperbound = 1000; 
	int lowerbound = 100; 
	int n = 3; 

	//logic for assigning ranges for each process
	int range = upperbound - lowerbound;
	int n_section = range/n;
	
	int sub_lowerbound;
	int sub_upperbound;
	
	int i;
	for(i = 1; i <= n; i++){
		
		pid_t pid = fork(); //Child Process
		
		if (pid < 0){
			fprintf(stderr,"Fork Failed");
			exit(1);
		}
		else if(pid == 0){

			if(i == 1){
				sub_lowerbound = lowerbound;
				sub_upperbound = sub_lowerbound + (i * n_section);
			}
				
			else if (i > 1 & i < n){
				sub_lowerbound = lowerbound + ((i-1) * n_section) + 1;
				sub_upperbound = lowerbound + ((i) * n_section);
			}
			
			else if (i == n){
				sub_lowerbound = lowerbound + ((i-1) * n_section) + 1;
				sub_upperbound = upperbound;
			}
			printf("Child %d checking range [%d, %d]\n", getpid(), sub_lowerbound, sub_upperbound);
            exit(0);
		}

	// Parent waits for all n number of children
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    printf("All children finished.\n");

    return 0;
}
