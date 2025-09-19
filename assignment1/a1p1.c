#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h> // For waitpid()
#include <stdlib.h>

int main()
{
    // Input Matrix of size 100 x 1000 
    int input_matrix[100][1000]; 
    int matrix_row = 0;
    int matrix_col = 0;

    pid_t pid; //pid value for fork calls
    pid_t pid_list[100];   // Stores PIDs of child processes in Step 2 and used in part 3
	int exit_value;        // Exit value set by each child in Step 2

    int fromChild;         // Exit status returned from child processes in Step 3
    int treasure_check = 0, treasure_col, treasure_row; // Variables to store where treasure is found in Step 3

  
    // Step 1: Read the input matrix from file
    for(int row = 0; row < 100; row++) {
        for(int col = 0; col < 1000; col++) {
            if(scanf("%d", &input_matrix[row][col]) != 1) {
                // Input must be strictly integers (0 or 1)
                fprintf(stderr, "Invalid Input file\n");
                exit(-1);
            }
        }
    }

   
    // Step 2: Create 100 child processes
    // Each child searches one row of the matrix
    while(matrix_row != 100) {
        pid = fork(); // Create a new process

        if (pid < 0) {
            // Fork failed
            fprintf(stderr, "Fork for Child %d failed", matrix_row);
            exit(-1);
        }
        else if (pid == 0) {
            // Child Process Code
            matrix_col = 0;
            exit_value = 0; 
            printf("Child %d (PID %d): Searching row %d\n", matrix_row, getpid(), matrix_row);

            // Search across the row for a treasure (non-zero value)
            while(matrix_col != 1000) {
                if(input_matrix[matrix_row][matrix_col]) {
                    exit_value = 1; // Indicate success
                }
                matrix_col++;
            }

            // Exit with either 0 (not found) or 1 (found)
            exit(exit_value);
        }
        else {
            // Parent Process Code
            // Store child PID in the list
            pid_list[matrix_row] = pid;
        }

        matrix_row++;
    }

    // Step 3: Parent waits for all children
    for(int row = 0; row < 100; row++) {
        waitpid(pid_list[row], &fromChild, 0); // Wait for specific child

        if(WEXITSTATUS(fromChild)) {
            // If child exited with success, scan row again in parent
            for(int col = 0; col < 1000; col++) {
                if(input_matrix[row][col]) {
                    treasure_col = col;
                    treasure_row = row;
                    treasure_check = 1;
                    break;
                }
            }
        }
    }

    // Step 4: Report Result
    if(!treasure_check) {
        fprintf(stderr, "Parent: No treasure found within the input matrix\n");
    }
    else {
        printf("Parent: The treasure was found by child with PID %d at row %d and column %d\n",
               pid_list[treasure_row], treasure_row, treasure_col);
    }

    printf("The parent is done\n");
    return 1;
}