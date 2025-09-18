#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>
#include <stdlib.h>

// Questions:
// Edge Cases:
// N should be greater than equal to 0, and an integer 
// lowerbound should be less than or equal to upperbound
// handle cases when N < number of range values


// function to test if input is a prime number
int is_prime(int num) {
	if (num < 2) return 0;
	for (int i = 2; i <= sqrt(num); i++) {
		if (num % i == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Invalid input. Correct order: %s LOWER_BOUND UPPER_BOUND N\n", argv[0]);
        exit(1);
    }

	int lowerbound = atoi(argv[1]);
    int upperbound = atoi(argv[2]);
    int n = atoi(argv[3]);

	
	//edge case 1: n <= 0.
	if(n <= 0){
		fprintf(stderr, "Invalid input. Enter N < 0.)";
        exit(1);
	}

	//edge case 2: lowerboundry > upperboundry
	if(lowerbound > upperbound){
		fprintf(stderr, "Invalid input. Enter LOWER_BOUND > UPPER_BOUND.");
        exit(1);
	}
	
	//logic for assigning ranges for each process
	int range = (upperbound - lowerbound) + 1;
	int n_section = range/n;
	
	int sub_lowerbound;
	int sub_upperbound;
	
	//memory layout suggested by assignment sheet
	int shm_size =  (n * range) * sizeof(int); // shared memory segment size
	int shmid = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0666); // shared memory creation
	int *shm_ptr = (int *) shmat(shmid, NULL, 0); // attaches shared memory block


	
	for(int i = 1; i <= n; i++){
		
		pid_t pid = fork(); //Child Process
		
		//ERROR
		if (pid < 0){
			fprintf(stderr,"Fork Failed");
			exit(1);
		}

		//CHILD
		else if(pid == 0){

			if(i == 1){
				sub_lowerbound = lowerbound;
				sub_upperbound = sub_lowerbound + (i * n_section) - 1;
			}
			else if (i > 1 && i < n){
				sub_lowerbound = lowerbound + ((i-1) * n_section);
				sub_upperbound = lowerbound + (i * n_section) - 1;
			}
			else if (i == n){
				sub_lowerbound = lowerbound + ((i-1) * n_section);
				sub_upperbound = upperbound;
			}
			
			printf("Child %d checking range [%d, %d]\n", getpid(), sub_lowerbound, sub_upperbound);

			
			int memory_block_index = i - 1;
			int shm_start_ind = memory_block_index * range;
			int shm_end_ind = (memory_block_index + 1) * range - 1;

			//checking if numbers in each range are prime numbers
			for(int number = sub_lowerbound; number <= sub_upperbound; number++){
				if( is_prime(number) == 1){
					shm_ptr[shm_start_ind] = number;
					shm_start_ind++;
					if (shm_start_ind > shm_end_ind) break;
				}
			}	
            exit(0);
		}
	}

	//Parent waits for all n number of children
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    printf("Parent: All children finished. Primes found:\n");

    // Parent reads shared memory
    for (int i = 0; i < n * range; i++) {
        if (shm_ptr[i] != 0) {
            printf("%d ", shm_ptr[i]);
        }
    }
    
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
   
    return 0;
    
}
