#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>


int main() {
	//memory
    //key_t key = ftok("shmfile", 10); //unique key for shared memory
    //int shmid = shmget(key, 1024, IPC_CREAT | 0666); //from sharedMem.c
    //int *shm_ptr = (int *) shmat(shmid, NULL, 0); //assignment code

	//parent child communication
	int toParent;
	int fromChild;

	//example input
	int upperbound = 200; 
	int lowerbound = 100; 
	int n = 3; 

	
	//logic for assigning ranges for each process
	int range = upperbound - lowerbound;
	int n_section = range/n;
	
	int sub_lowerbound;
	int sub_upperbound;
	
	int i;
	for(i = 1; i <= 1; i++){
		sub_lowerbound = lowerbound;
		sub_upperbound = sub_lowerbound + (i * n_section);
		printf("%d, %d\n",sub_lowerbound,sub_upperbound);
	}


   
    // from assignment page
    int is_prime(int num) {
        if (num < 2) return 0;
            for (int i = 2; i <= sqrt(num); i++) {
                if (num % i == 0) return 0;
            }
        return 1;
      }

    
    
  //   pid_t pid = fork();

  //   if (pid < 0) {
		// fprintf(stderr, "Fork Failed");
		// exit(-1);
  //   }
    
  //   else if (pid == 0) {
  //       sleep(1); // waiting so parent doesn't execute  
	 //  }

  //   else {
  //       wait(&fromChild);
		// printf("done", WEXITSTATUS(fromChild));
  //   }

    return 0;
}
