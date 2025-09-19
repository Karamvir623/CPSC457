#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>

//Note:
//for decimal inputs: the program will output primes for the floor of the lowerbound, upperbound, and n value.


//Function: is_prime
//Purpose : 
// 1. checks if number is prime
// 2. returns 1 if a number is prime, 0 otherwise
int is_prime(int num) {
	if (num < 2) return 0;
	for (int i = 2; i <= sqrt(num); i++) {
		if (num % i == 0) return 0;
	}
	return 1;
}

// Function: main
// Purpose: 
// 1. divides a range [LOWER_BOUND, UPPER_BOUND] across N child processes, each checking for prime numbers in its subrange.
// 2. results are stored in shared memory and printed by the parent after all children finish.
int main(int argc, char *argv[]) {

	// edge case 1: ensures exactly 3 arguments are provided in the correct order (program + 3 inputs)
	if (argc != 4) {
        fprintf(stderr, "Invalid input. Correct order: %s LOWER_BOUND UPPER_BOUND N\n", argv[0]);
        exit(1);
    }

	// initializes values and converts command line arguments to integers
	int lowerbound = atoi(argv[1]); //lowerbound of range
    int upperbound = atoi(argv[2]); //upperbound of range
    int n = atoi(argv[3]); // n number of processes


	//edge case 2: n <= 0. ensures number of processes entered is a positive integer
	if(n <= 0){
		fprintf(stderr, "Invalid input. Enter N > 0.\n");
        exit(1);
	}

	//edge case 2: ensures lowerboundry must be less than or equal to upperboundry
	if(lowerbound > upperbound){
		fprintf(stderr, "Invalid input. Enter LOWER_BOUND <= UPPER_BOUND.\n");
        exit(1);
	}

	//edge case 3: ensures both lowerboundry and upperboundry are greater than 0
	if(lowerbound < 0 || upperbound < 0){
		fprintf(stderr, "Invalid input. Enter LOWER_BOUND and UPPER_BOUND > 0.\n");
        exit(1);
	}
	
	//variables used to determine subranges
	int range = (upperbound - lowerbound) + 1; // calculates the number of values in the range (inclusive)
	int n_section = range/n; // dividing the range into equal sections based on n (numbers per sub range)
	
	int sub_lowerbound = 0; // will hold each child’s lower bound
	int sub_upperbound = 0; // will hold each child’s upper bound

	//edge case 4: ensures extra processes are not created if each number in the entered range has been checked.
    if(n > range){
        n_section = 1; // each process handles exactly 1 number
        n = range; // reduce number of processes to match numbers
    }
	
	// shared memory setup
	// instead of using a fixed value, MAX_PRIMES_PER_CHILD is equal to n * range 
	// since every value in the input range will notbe a prime value, this will ensure there is 
	// enough memory for each prime number per child can be written to memory
	int shm_size =  (n * range) * sizeof(int); // shared memory size (ensures enough memory per child) 
	int shmid = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0666); // shared memory creation
	int *shm_ptr = (int *) shmat(shmid, NULL, 0); // attaches shared memory to process

	// fork child process
	for(int i = 0; i <= n - 1; i++){ //creates n children
		
		pid_t pid = fork(); // creates child process
		
		//fork failed
		if (pid < 0){
			fprintf(stderr,"Fork Failed\n");
			exit(1);
		}

		//child process
		else if(pid == 0){
			//assigns a subrange for each child
			
			if(i == 0){ // for first subrange 
                sub_lowerbound = lowerbound;
                sub_upperbound = lowerbound + ((i + 1) * n_section) - 1;
            }
            else if (i > 0 && i < n - 1){ // for subranges inbetween 
                sub_lowerbound = lowerbound + (i * n_section);
                sub_upperbound = lowerbound + ((i + 1) * n_section) - 1;    
            }
            else if (i == n - 1){ // for last subrange to ensure all values are included (including remainder)
                sub_lowerbound = lowerbound + (i * n_section);
                sub_upperbound = upperbound;
            }

			// shows child process info
			printf("Child PID %d checking range [%d, %d]\n", getpid(), sub_lowerbound, sub_upperbound);

			// computes where this child writes results in shared memory (ensuring no overlap)
			// child 1 mem [start...end] child 2 mem [start...end] ... child N mem [start...end] 
			int shm_ind = i; // index for shared memory block 
			int shm_start_ind = shm_ind * range; // start of memory for n child
			int shm_end_ind = (shm_ind + 1) * range - 1;//end of memory for n child 

			// checking if numbers in each range are prime numbers
			for(int number = sub_lowerbound; number <= sub_upperbound; number++){
				if( is_prime(number) == 1){ // if prime, writes to shared memory
					shm_ptr[shm_start_ind] = number;
					shm_start_ind++;
					if (shm_start_ind > shm_end_ind) break; //if there is overflow
				}
			}	
            exit(0); // child finishes
		}
	}

	// parent process, waits for all children to finish
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

	// parent reads shared memory
    printf("\nParent: All children finished. Primes found:\n");
    for (int i = 0; i < n * range; i++) {
        if (shm_ptr[i] != 0) { // shm default is 0
            printf("%d ", shm_ptr[i]);
        }
    }
	 printf("\n");

	// detaches and removes shared memory
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0; 
}
