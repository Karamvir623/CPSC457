#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h> // ask if we can use ceil/floor function

int main() {
    //key_t key = ftok("shmfile", 10); //unique key for shared memory
    //int shmid = shmget(key, 1024, IPC_CREAT | 0666); //from sharedMem.c
    //int *shm_ptr = (int *) shmat(shmid, NULL, 0); //assignment code

    int upperbound;
    int lowerbound;
    int nprocess;

    int toParent;
	  int fromChild;

    //for testing
    printf("Enter lower bound: ");
    scanf("%d", &lowerbound);

    printf("Enter upper bound: ");
    scanf("%d", &upperbound);

    printf("Enter number of N processes performed: ");
    scanf("%d", &nprocess);

    // from assignment page
    int is_prime(int num) {
        if (num < 2) return 0;
            for (int i = 2; i <= sqrt(num); i++) {
                if (num % i == 0) return 0;
            }
        return 1;
      }

    // assigning ranges to each process logic
    
    pid_t pid = fork();

    if (pid < 0) {
		fprintf(stderr, "Fork Failed");
		exit(-1);
    }
    
    else if (pid == 0) {
        sleep(1); // waiting so parent doesn't execute  
	  }

    else {
        wait(&fromChild);
		printf("done", WEXITSTATUS(fromChild));
    }

    return 0;
}
